# Change the following as required:
CC	= c++
CFLAGS	= -Wall -I../include/
OBJDIR=../objects/
INCLDIR=../include/

all: oc_IMap ${OBJDIR}oc_IMap.o ${INCLDIR}ocSocket.h ${INCLDIR}oc_IMap.h ${INCLDIR}oc_SMTP.h

oc_IMap: ${OBJDIR}oc_IMap.o 
	${CC} ${CFLAGS} -o $@ ${OBJDIR}oc_IMap.o

${OBJDIR}oc_IMap.o: oc_IMap.cpp ocSocket.h
	${CC} ${CFLAGS} -c oc_IMap.cpp -o ${OBJDIR}oc_IMap.o

${INCLDIR}ocSocket.h: ocSocket.h
	cp ocSocket.h ${INCLDIR}

${INCLDIR}oc_IMap.h: oc_IMap.h
	cp oc_IMap.h ${INCLDIR}
  
${INCLDIR}oc_SMTP.h: oc_SMTP.h
	cp oc_SMTP.h ${INCLDIR}  

clean:
	rm T2
	rm oc_IMap
