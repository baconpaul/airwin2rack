# Just messin around

What do I need to script:

1. copy libs/airwindows/plugins/MacVST/(name)/source/*.cpp src/autogen_airwin

Header File

1. replace "audioeffectx.h" with "../airwin2rackbase.h"
2. add a `namespace airwin2rack::(name)` at the first enum
3. replace virtual void getParameterName with static void getParameterName

.cpp file

1. add a `namespace airwin2rack::(name)` after the endif

Proc.cpp file

1. add a `namespace airwin2rack::(name)` after the endif
