package main

import (
	"log"
	"net"

	//    "encoding/hex"
	"os"
	"sync"
	"time"
)

type client struct {
    addr net.Addr
    pc net.PacketConn
}

var connections map[string]client
var connMutex sync.Mutex

func main() {
    args := os.Args
    if len(args) == 1 {
        log.Fatal("missing argument: port")
    }
    pc, err := net.ListenPacket("udp", ":" + args[1])
    if err != nil {
        log.Fatal(err)
    }
    defer pc.Close()

    msgs := make(chan []byte, 2000)
    connections = make(map[string]client)

    go sendToAll(msgs)

    go func(){ 
        for {
            time.Sleep(1 * time.Second)
            log.Println("connections:", len(connections))
        }
    }()

    for {
        buf := make([]byte, 1024)
        n, addr, err := pc.ReadFrom(buf)
        if err != nil {
            continue
        }
        go serve(pc, addr, buf[:n], msgs)
    }
}

func serve(pc net.PacketConn, addr net.Addr, buf []byte, msgs chan<- []byte) {
    // if the second byte is a 2, its a disconnect package, and we should remove that client
    if (buf[1] == 0x2) {
        log.Println("disconnected:", addr.String())
        connMutex.Lock()
        delete(connections, addr.String())
        connMutex.Unlock()
    } else {
        addrStr := addr.String()
        connMutex.Lock()
        if _, ok := connections[addrStr]; !ok {
            log.Println("connected:", addrStr)
            connections[addrStr] = client { addr, pc }
        }
        connMutex.Unlock()
    }
    //log.Printf("%s: %s", addr.String(), hex.EncodeToString(buf))
    msgs <- buf
}

func sendToAll(msgs <-chan []byte) {
    for {
        msg := <-msgs
        connMutex.Lock()
        for _, theClient := range connections {
            theClient.pc.WriteTo(msg, theClient.addr)
        }
        connMutex.Unlock()
    }
}
