///
/// !Usage
/// -# Initialize the DBGU using TRACE_CONFIGURE() if you intend to eventually
///    disable ALL traces; otherwise use DBGU_Configure().
/// -# Uses the TRACE_DEBUG(), TRACE_INFO(), TRACE_WARNING(), TRACE_ERROR()
///    TRACE_FATAL() macros to output traces throughout the program.
/// -# Each type of trace has a level : Debug 5, Info 4, Warning 3, Error 2
///    and Fatal 1. Disable a group of traces by changing the value of
///    TRACE_LEVEL during compilation; traces with a level bigger than TRACE_LEVEL
///    are not generated. To generate no trace, use the reserved value 0.
/// -# Trace disabling can be static or dynamic. If dynamic disabling is selected
///    the trace level can be modified in runtime. If static disabling is selected
///    the disabled traces are not compiled.
///
/// !Trace level description
/// -# TRACE_DEBUG (5): Traces whose only purpose is for debugging the program,
///    and which do not produce meaningful information otherwise.
/// -# TRACE_INFO (4): Informational trace about the program execution. Should
///    enable the user to see the execution flow.
/// -# TRACE_WARNING (3): Indicates that a minor error has happened. In most case
///    it can be discarded safely; it may even be expected.
/// -# TRACE_ERROR (2): Indicates an error which may not stop the program execution,
///    but which indicates there is a problem with the code.
/// -# TRACE_FATAL (1): Indicates a major error which prevents the program from going
///    any further.

//------------------------------------------------------------------------------

#ifndef TRACE_H
#define TRACE_H

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------
//#include <dbgu.h>
#include <stdio.h>

//------------------------------------------------------------------------------
//         Global Definitions
//------------------------------------------------------------------------------

#define TRACE_LEVEL_DEBUG      5
#define TRACE_LEVEL_INFO       4
#define TRACE_LEVEL_WARNING    3
#define TRACE_LEVEL_ERROR      2
#define TRACE_LEVEL_FATAL      1
#define TRACE_LEVEL_NO_TRACE   0

// By default, all traces are output except the debug one.
#if !defined(TRACE_LEVEL)
#define TRACE_LEVEL TRACE_LEVEL_INFO
#endif

// By default, trace level is static (not dynamic)
#if !defined(DYN_TRACES)
#define DYN_TRACES 0
#endif

#if defined(NOTRACE)
#error "Error: NOTRACE has to be not defined !"
#endif

#undef NOTRACE
#if (TRACE_LEVEL == TRACE_LEVEL_NO_TRACE)
#define NOTRACE
#endif



//------------------------------------------------------------------------------
//         Global Macros
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Outputs a formatted string using <printf> if the log level is high
/// enough. Can be disabled by defining TRACE_LEVEL=0 during compilation.
/// \param format  Formatted string to output.
/// \param ...  Additional parameters depending on formatted string.
//------------------------------------------------------------------------------
#if defined(NOTRACE)

// Empty macro
#define TRACE_DEBUG(...)      { }
#define TRACE_INFO(...)       { }
#define TRACE_WARNING(...)    { }
#define TRACE_ERROR(...)      { }
#define TRACE_FATAL(...)      { HaltCpu(0); }

#define TRACE_DEBUG_WP(...)   { }
#define TRACE_INFO_WP(...)    { }
#define TRACE_WARNING_WP(...) { }
#define TRACE_ERROR_WP(...)   { }
#define TRACE_FATAL_WP(...)   { HaltCpu(0); }

#elif (DYN_TRACES == 1)

// Trace output depends on traceLevel value
#define TRACE_DEBUG(...)      { if (traceLevel >= TRACE_LEVEL_DEBUG)   { printf("-D- " __VA_ARGS__); } }
#define TRACE_INFO(...)       { if (traceLevel >= TRACE_LEVEL_INFO)    { printf("-I- " __VA_ARGS__); } }
#define TRACE_WARNING(...)    { if (traceLevel >= TRACE_LEVEL_WARNING) { printf("-W- " __VA_ARGS__); } }
#define TRACE_ERROR(...)      { if (traceLevel >= TRACE_LEVEL_ERROR)   { printf("-E- " __VA_ARGS__); } }
#define TRACE_FATAL(...)      { if (traceLevel >= TRACE_LEVEL_FATAL)   { printf("-F- " __VA_ARGS__); HaltCpu(0); } }

#define TRACE_DEBUG_WP(...)   { if (traceLevel >= TRACE_LEVEL_DEBUG)   { printf(__VA_ARGS__); } }
#define TRACE_INFO_WP(...)    { if (traceLevel >= TRACE_LEVEL_INFO)    { printf(__VA_ARGS__); } }
#define TRACE_WARNING_WP(...) { if (traceLevel >= TRACE_LEVEL_WARNING) { printf(__VA_ARGS__); } }
#define TRACE_ERROR_WP(...)   { if (traceLevel >= TRACE_LEVEL_ERROR)   { printf(__VA_ARGS__); } }
#define TRACE_FATAL_WP(...)   { if (traceLevel >= TRACE_LEVEL_FATAL)   { printf(__VA_ARGS__); HaltCpu(0); } }

#else

// Trace compilation depends on TRACE_LEVEL value
#if (TRACE_LEVEL >= TRACE_LEVEL_DEBUG)
#define TRACE_DEBUG(...)      { printf("-D- " __VA_ARGS__); }
#define TRACE_DEBUG_WP(...)   { printf(__VA_ARGS__); }
#else
#define TRACE_DEBUG(...)      { }
#define TRACE_DEBUG_WP(...)   { }
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_INFO)
#define TRACE_INFO(...)       { printf("-I- " __VA_ARGS__); }
#define TRACE_INFO_WP(...)    { printf(__VA_ARGS__); }
#else
#define TRACE_INFO(...)       { }
#define TRACE_INFO_WP(...)    { }
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_WARNING)
#define TRACE_WARNING(...)    { printf("-W- " __VA_ARGS__); }
#define TRACE_WARNING_WP(...) { printf(__VA_ARGS__); }
#else
#define TRACE_WARNING(...)    { }
#define TRACE_WARNING_WP(...) { }
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_ERROR)
#define TRACE_ERROR(...)      { printf("-E- " __VA_ARGS__); }
#define TRACE_ERROR_WP(...)   { printf(__VA_ARGS__); }
#else
#define TRACE_ERROR(...)      { }
#define TRACE_ERROR_WP(...)   { }
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_FATAL)
#define TRACE_FATAL(...)      { printf("-F- " __VA_ARGS__); while(1); }
#define TRACE_FATAL_WP(...)   { printf(__VA_ARGS__); while(1); }
#else
#define TRACE_FATAL(...)      { HaltCpu(0); }
#define TRACE_FATAL_WP(...)   { HaltCpu(0); }
#endif

#endif


//------------------------------------------------------------------------------
//         Exported variables
//------------------------------------------------------------------------------
// Depending on DYN_TRACES, traceLevel is a modifable runtime variable
// or a define
#if !defined(NOTRACE) && (DYN_TRACES == 1)
    extern unsigned int traceLevel;
#endif

#endif //#ifndef TRACE_H

