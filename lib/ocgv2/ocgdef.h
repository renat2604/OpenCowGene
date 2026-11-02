#ifndef DEFINES_H
#define DEFINES_H

#define DEFINE_STRUCT_TYPE(name) \
	typedef struct name name

#define DEFINE_CALLBACK(name, return_type, ...) \
	typedef return_type (*name)(__VA_ARGS__)

#endif
