package main

import (
	"log"
	"net"
	"os"
	"sync"
	"time"
    "os/signal"
    "context"
)

type client struct {
    addr net.Addr
    pc net.PacketConn
}

var connections map[string]client
var connMutex sync.Mutex
var closeServer = false

func main() {
    args := os.Args
    if len(args) == 1 {
        log.Fatal("missing argument: port")
    }

    // capture Ctrl+C
    sigChan := make(chan os.Signal, 1)
    signal.Notify(sigChan, os.Interrupt)

    ctx, cancelFunc := context.WithCancel(context.Background())

    go func(ctx context.Context, cancelFunc context.CancelFunc) {
        <-sigChan
        log.Println("closing server...")
        cancelFunc() // cancel context work
    }(ctx, cancelFunc)

    pc, err := net.ListenPacket("udp", ":" + args[1])
    if err != nil {
        log.Fatal(err)
    }
    defer pc.Close()

    msgs := make(chan []byte, 2000)
    connections = make(map[string]client)

    go sendToAll(ctx, msgs)

    go func(ctx context.Context) { 
        for {
            select {
            case <-ctx.Done():
                return
            default:
            }
            time.Sleep(1 * time.Second)
            log.Println("connections:", len(connections))
        }
    }(ctx)

    for {
        select {
        case <-ctx.Done():
            return
        default:
        }
        buf := make([]byte, 1024)
        beDoneBy := time.Now()
        beDoneBy = beDoneBy.Add(time.Second * 2)
        pc.SetReadDeadline(beDoneBy)
        n, addr, err := pc.ReadFrom(buf)
        if err != nil {
            log.Println("read timed out")
            continue
        }
        go serve(ctx, pc, addr, buf[:n], msgs)
    }
}

func serve(ctx context.Context, pc net.PacketConn, addr net.Addr, buf []byte, msgs chan<- []byte) {
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
    msgs <- buf
}

func sendToAll(ctx context.Context, msgs <-chan []byte) {
    for {
        select {
        case <-ctx.Done():
            return
        case msg := <-msgs:
            connMutex.Lock()
            for _, theClient := range connections {
                theClient.pc.WriteTo(msg, theClient.addr)
            }
            connMutex.Unlock()
        }
    }
}
