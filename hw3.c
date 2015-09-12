//Mia Armstrong
//CS 201 Assignment 3
//Filename: hw3.c
/* This is a decryption program for the decryption key - ARM This program will decrypt a file using the specifications in the assignment. The program will store a file in memory and then decrypt the file and output the result to the screen.*/

#include <stdio.h>
#include <stdlib.h>


//function declarations
int fileexists(const char * filename);
int fsize(const char * filename);
unsigned char swapBits(unsigned char x, unsigned int p1, unsigned int p2, unsigned int n);
unsigned char flipBytes(unsigned char x);


int main(int argc, char * argv[])
{
   long long size = 0; //file size
   int i = 0; //index
   FILE * fp; //file pointer
   char * buffer; //buffer
   size_t result; //copy file into buffer
   unsigned char temp; //hold a char in the buffer

   if(argc != 2) //to check for file name
   {
      fprintf(stderr, "Specify a filename.\n");
      exit(1);
   }

   if (fileexists(argv[1]) == 0)  // to check if file exists
   {
      fprintf(stderr, "File does not exist.\n");
      exit(1);
   }

   size = fsize(argv[1]);  //to get file size 
   if (size == -1)
   {
      exit(1);
   }


   fp = fopen (argv[1], "rb"); //open the file
   
   buffer = (char *) malloc(sizeof(char)*size); //allocate space on the heap

   if (buffer == NULL) //error detection if file is empty
   {
      fputs("Memory error",stderr);
      exit(2);
   }
   result = fread(buffer, sizeof(*buffer), size, fp); //read file into memory
   if (result != size)
   {
      fputs ("Reading error",stderr);
      exit(3);
   }

   //stage 1 swap byte c0 and c3
   for (i=0; i<size; ++i)
   {
      if (0x00 == buffer[i] || 0x00 == buffer[i+3]) //if bytes do not exist
      {
         break;
      }
      temp = buffer[i];
      buffer[i] = buffer[i+3];
      buffer[i+3] = temp;
      i = i + 3;
      
   }

   printf("\n\n");
      
   //stage 2 swap bits b7 and b5, swap bits b6 and b4
   for (i=0; i<size; ++i)
   {
      buffer[i] = swapBits(buffer[i], 5, 7, 1);
      buffer[i] = swapBits(buffer[i],4,6,1);
   }
   
   // stage 3 xor bytes c1 and c3 with M
   for (i=0; i<size; ++i)
   {
      if (0x00 == buffer[i+1] || 0x00 == buffer[i+3])//if bytes do not exist
      {
         break;
      }
      buffer[i+1] = flipBytes(buffer[i+1]);
      buffer[i+3] = flipBytes(buffer[i+3]);
      i = i + 3;
   }

   printf("%s\n\n", buffer);

   fclose(fp);
   free(buffer);
   return(0);
}

//This function takes in a filename and returns an int.
//Returns 1 if the file exists or 0 if the file does not exist
int fileexists(const char * filename)
{
   FILE * file;
   file = fopen(filename, "r");
   if (file == NULL)
   {
      fputs("File Error", stderr);
   }
   else
   {
      fclose(file);
      return 1;
   }
   return 0;
}

//This function takes in a filename and returns an int
//Returns file size or -1 if the size cannot be determined.
int fsize(const char * filename)
{
   int size = 0;
   FILE * file;

   file = fopen(filename, "rb"); //binary mode
   if (file != NULL)
   {
      if (fseek(file, 0, SEEK_END))
      {
         fclose(file);
         fprintf(stderr, "Cannot determine size of %s.\n",
         filename);
         return -1;
      }
      
      size = ftell(file);
      rewind(file);
      fclose(file);
      return size;
   }
   return -1; //error
}

//This function takes in an unsigned char, and three ints. 
//This function swaps two bits 
//Returns an unsigned char
unsigned char swapBits(unsigned char x, unsigned int p1, unsigned int p2, unsigned int n)
{
   unsigned int swap = ((x >> p1) ^ (x >> p2)) & ((1U << n) -1);

   return x ^ ((swap << p1) | (swap << p2));
}

//This function takes in an unsigned char
//This function xors a byte and a char from the key
//Returns an unsigned char
unsigned char flipBytes(unsigned char x)
{
   x = x ^ 0x6D;
   return x;
}
