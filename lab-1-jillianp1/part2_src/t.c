#include <stdio.h>
#include <fcntl.h>

#include <sys/types.h>
#include <unistd.h>

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

// please put your main and code in this file. All contained.
// Use the provided mk script to compile your code. You are welcome to change the mk script if you'd like
// but know that is how we will be batch compiling code using mk on the back end so your code will need to be able
// to run when that bash script is ran.
// Most code can be grabbed from t.c in root of the github repo. 

struct partition {
	u8 drive;             // drive number FD=0, HD=0x80, etc.

	u8  head;             // starting head 
	u8  sector;           // starting sector
	u8  cylinder;         // starting cylinder

	u8  sys_type;         // partition type: NTFS, LINUX, etc.

	u8  end_head;         // end head 
	u8  end_sector;       // end sector
	u8  end_cylinder;     // end cylinder

	u32 start_sector;     // starting sector counting from 0 
	u32 nr_sectors;       // number of of sectors in partition
  };

char *dev = "vdisk";
int fd;
    
// read a disk sector into char buf[512]
int read_sector(int fd, int sector, char *buf)
{
    lseek(fd, sector*512, SEEK_SET);  // lssek to byte sector*512
    read(fd, buf, 512);               // read 512 bytes into buf[ ]
}

int main()
{
    struct partition *p;
    char buf[512]; //sector 0
    fd = open(dev, O_RDONLY);   // open dev for READ
    read(fd, buf, 512);         // read 512 bytes of fd into buf[ ] 

    p = (struct partition *)&buf[0x1BE];
    printf("    start sector    end sector      nr_sector   sys type\n");
    int partCount = 1;
    while(p->sys_type != 0)
    {
        printf("P%d :       %d          %d          %d          %x\n", partCount, p->start_sector, p->end_sector, p->nr_sectors, p->sys_type);
        //if the sys type is 5 its extend type
        if(p->sys_type == 5)
        {
            printf("This is extend type: P%d\n", partCount);
            //when start sector is 0 there is no more
            if(p->start_sector == 0)
            {
              return;
            }
            int extStart = p->start_sector;
            printf("start_sector: %d\n", extStart);
            int localMBR = p->start_sector;
            printf("next localMBR sector= %d\n", localMBR);
            printf("--------------------------------------------\n");
            int entryCount = 1;
            int newPartCount = partCount +1;
            while(p->sys_type != 0)
            {
                read_sector(fd, localMBR, buf);
                p = (struct partition *)&buf[0x1BE];
                //these are the partitions
                printf("P%d:\n", newPartCount);
                printf("Entry%d : Start Sector= %d nr_sectors= %d\n", entryCount, p->start_sector + localMBR, p->nr_sectors);
                entryCount++;
                p++;
                newPartCount++;
                localMBR = extStart +  p->start_sector;
                printf("Next local MBR sector: %d + %d = %d\n",extStart, p->start_sector, localMBR);
                printf("-------------------------------------------\n");
            }
        }
        else
        {
            partCount++;
            p++; //move to point to next partition table in MBR
        }
    }

}
