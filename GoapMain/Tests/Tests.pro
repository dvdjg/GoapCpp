TEMPLATE = subdirs

SUBDIRS += \
    FactoryTest \
    DataTest \
    BotanTest \
    RpcTest \
    RpcExamples

!win32-g++:SUBDIRS += ArrayFireTest

# Very slow compilation
#    GlooxTest \

#    RpcTest \
#    MsgPackTest \


# $ for a in `ls -1 *.cpp`; do n=${a%_test.*}; echo -e "TARGET = tst_gloox_$n\n\ninclude(../GlooxTest.pri)\n\nSOURCES += main.cpp ${a}\n" > ${n}_test.pri; done
# for a in `ls -1 *.cpp`; do n=${a%_test.*}; mkdir ${n}_test; mv -f ${n}_test.* ${n}_test ; done
