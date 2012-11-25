CC=g++ 
CFLAG=-g 

rename: util.o decodeinfo_mp3.o scan_dir.o rename_mp3.o
	$(CC) $(CFLAG) util.o decodeinfo_mp3.o scan_dir.o rename_mp3.o -o rename
rename_mp3.o: rename_mp3.cpp scan_dir.h decodeinfo_mp3.h
	$(CC) $(CFLAG) -c rename_mp3.cpp -o rename_mp3.o
decodeinfo_mp3.o: decodeinfo_mp3.cpp decodeinfo_mp3.h scan_dir.h
	$(CC) $(CFLAG) -c decodeinfo_mp3.cpp -o decodeinfo_mp3.o
scan_dir.o: scan_dir.cpp scan_dir.h
	$(CC) $(CFLAG) -c scan_dir.cpp -o scan_dir.o
util.o: util.cpp util.h
	$(CC) $(CFLAG) -c util.cpp -o util.o

ut: util.o decodeinfo_mp3.o scan_dir.o ut.o
	$(CC) $(CFLAG) util.o decodeinfo_mp3.o scan_dir.o ut.o -o ut
ut.o: ut.cpp scan_dir.h decodeinfo_mp3.h
	$(CC) $(CFLAG) -c ut.cpp -o ut.o

clean:
	rm decodeinfo_mp3.o scan_dir.o rename_mp3.o ut.o util.o rename ut

