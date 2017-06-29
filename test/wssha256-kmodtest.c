/**
 * @file   testwssha256kern.c
 * @author Brett Nicholas
 * @date   7 April 2015
 * @version 0.1
 * @brief  A Linux user space program that communicates with the wssha256kern.c LKM. It passes a
 * string to the LKM and reads the response from the LKM. For this example to work the device
 * must be called /dev/wssha256char.
 */
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define SHA256_MSG_SIZE 256
#define SHA256_DGST_SIZE 32

static volatile char digest[SHA256_DGST_SIZE];     ///< The receive buffer from the LKM
static char messages[3][SHA256_MSG_SIZE];
static const char correct[3][SHA256_DGST_SIZE] = { 
  {0x67,0xf0,0x22,0x19,0x5e,0xe4,0x05,0x14,0x29,0x68,0xca,0x1b,0x53,0xae,0x25,0x13,0xa8,0xba,0xb0,0x40,0x4d,0x70,0x57,0x77,0x85,0x31,0x6f,0xa9,0x52,0x18,0xe8,0xba},
  {0x6c,0x50,0x76,0x06,0x1b,0x0c,0xc3,0x1f,0x39,0x87,0x76,0x7c,0x06,0x2c,0xd1,0x33,0xab,0x13,0x07,0x34,0xa0,0xb8,0x18,0x4c,0x65,0xd0,0x65,0x88,0x18,0x23,0xb9,0x92 },
  {0x87,0x9a,0x0a,0xa9,0xbb,0xa8,0x7c,0x38,0x8e,0x07,0x0f,0xd7,0x04,0xce,0xa2,0x50,0x20,0xa4,0x57,0x39,0x60,0x7b,0x6f,0xa0,0xbe,0x5d,0xbf,0x98,0x78,0x66,0x26,0x2c }};

int main()
{
  int ret, fd, errcnt;

  // create the three test vectors, one with all (ASCII) zeros, one with ABCDEFGH.., 
  // and one with ABCDEFGH... but with an ASCCI zero in the last element
  memset((void*)messages[0],'0',SHA256_MSG_SIZE);
  for (int i=0; i<SHA256_MSG_SIZE; i++) 
  {
    messages[1][i] = 'A'+(i%26);
    messages[2][i] = messages[1][i];
  }
  messages[2][SHA256_MSG_SIZE-1] = '0'; 

  // initialize digest to all zeros
  memset((void*)digest,0,SHA256_DGST_SIZE);

  // Open the device with read/write access
  printf("Starting device test code example...\n");
  fd = open("/dev/wssha256char", O_RDWR);             
  if (fd < 0){
    perror("Failed to open the device...");
    return errno;
  }

  errcnt=0;

  // Run the three test cases
  for (int testnum=0; testnum<3; testnum++)
  {
    printf("Test vector [%d]\n",testnum);
    
    // Print message to hash
    printf("\tMessage to hash: ");
    for (int i=0; i<SHA256_MSG_SIZE; i++) 
        printf("%c",messages[testnum][i]);
    printf("\n");

    // send the test vector to LKM
    ret = write(fd, messages[testnum], SHA256_MSG_SIZE);
    if (ret < 0){
      perror("Failed to write the message to the device.");
      return errno;
    }

    // read back the response from the LKM and print
    ret = read(fd, digest, SHA256_DGST_SIZE);        
    if (ret < 0){
      perror("Failed to read the message from the device.");
      return errno;
    }
    printf("\tThe received digest is: ");
    for (int i=0; i<SHA256_DGST_SIZE; i++)
      printf("%X ", digest[i]);
    printf("\n\n");

    // check against known solution
    for (int i=0; i<SHA256_DGST_SIZE; i++)
    {
      if (digest[i] != correct[testnum][i])
      {
        errcnt++;
        printf("\t****Error, incorrect digest value for test vector %d, element %i!\n",testnum,i);

      }
    }

    // report erroneous values
    if (errcnt == 0)
      printf("****Test vector %d status: SUCCESS\n\n",testnum);
    else 
    {
      printf("****Test vector %d status: FAILED\n\n",testnum);
      return -1; 
    }
      
    // reset digest to all zeros
    memset((void*)digest,0,SHA256_DGST_SIZE);
  }

  // close and exit
  if(close(fd)<0)
    perror("Error closing file");

  return 0;
}
