The following directories contain Vale and F\* sources:

* [specs](./specs): Trusted specification files that cover [basic definitions](./specs/defs/),
                    [math](./specs/defs/), [cryptographic algorithms](./specs/crypto/),
                    and [hardware assumptions](./specs/hardware/)
* [code](./code): Verified F\* support libraries and verified Vale cryptographic code
    * [arch](./code/arch): Optimized libraries for reasoning about hardware operations
    * [crypto](./code/crypto): verified cryptographic code
    * [test](./code/test): test files 
    * [thirdPartyPorts](./code/thirdPartyPorts): Code ported into Vale from external sources

Building will create the following additional directories;
all files generated by the build should be in these directories:

* obj
* bin
