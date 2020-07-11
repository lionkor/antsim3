#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
 * This file defines constant strings and other constant values used throughout the engine
 */

#define CstString static constexpr const char*

CstString cst_error_multiple_unique_components_fmt = "Attempted to add a Unique component with uuid {} to entity uuid {}, but another component of this type ({}) exists already, this is considered a fatal error";
CstString cst_error_multiple_unique_components = "Attempted to add a Unique component, but another component of this type exists already";

#endif // CONSTANTS_H
