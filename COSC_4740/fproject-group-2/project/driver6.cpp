
/* Driver 6*/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "disk.h"
#include "diskmanager.h"
#include "partitionmanager.h"
#include "filesystem.h"
#include "client.h"

using namespace std;

/*
  This driver will test the getAttributes() and setAttributes()
  functions. You need to complete this driver according to the
  attributes you have implemented in your file system, before
  testing your program.
*/

int main()
{

  Disk *d = new Disk(300, 64, const_cast<char *>("DISK1"));
  DiskPartition *dp = new DiskPartition[3];

  dp[0].partitionName = 'A';
  dp[0].partitionSize = 100;
  dp[1].partitionName = 'B';
  dp[1].partitionSize = 75;
  dp[2].partitionName = 'C';
  dp[2].partitionSize = 105;

  DiskManager *dm = new DiskManager(d, 3, dp);
  FileSystem *fs1 = new FileSystem(dm, 'A');
  FileSystem *fs2 = new FileSystem(dm, 'B');
  FileSystem *fs3 = new FileSystem(dm, 'C');
  Client *c1 = new Client(fs1);
  Client *c2 = new Client(fs2);
  Client *c3 = new Client(fs3);
  Client *c4 = new Client(fs1);
  Client *c5 = new Client(fs2);



  int r;

/*
What every need to show your set and get Attributes functions work*/

 /*I tried to include some test cases to validate the functions.*/
/*
  cout << "\n\n\n\nAttributes are name, name length, color, and file rank (must be greater than 0). \nAny two files can have the same rank / color.\n";
  cout << "COLOR KEY: 1 = Red \t 2 = Blue \t 3 = Yellow\n\n";
  
  
  r = c1->myFS->setAttributes(const_cast<char *>("/e/f"),2, 'r', 20);
  cout << (r>=0?"Correct, attributes are set": " Failure, check parameters") <<endl;
  r = c4->myFS->setAttributes(const_cast<char *>("/e/b"),2, 'b', 30);
  cout << (r>=0?"Correct, attributes are set": " Failure, check parameters") <<endl;
  r = c1->myFS->getAttributes(const_cast<char *>("/e/f"),2, 1);
  cout << "Requested attribute is " << r << (r>=0?"; this is correct": "; failure, check parameters") <<endl;
  r = c4->myFS->getAttributes(const_cast<char *>("/e/b"), 2,'g', 2); //Should fail (invalid color)
  cout << "Requested attribute is " << r << (r>=0?"; this is correct": "; failure, check parameters") <<endl;
  r = c1->myFS->getAttributes(const_cast<char *>("/p"),2, 1);  //should failed!
  cout << "Requested attribute is " << r << (r>=0?"; this is correct": "; failure, check parameters") <<endl;
  r = c4->myFS->setAttributes(const_cast<char *>("/p"),2, 'r', 45);  //should failed!
  cout << (r>=0?"Correct, attributes are set": " Failure, check parameters") <<endl;
  
  r = c2->myFS->setAttributes(const_cast<char *>("/f"),2, 1, 45); //Should fail (invalid color)
  cout << (r>=0?"Correct, attributes are set": " Failure, check parameters") <<endl;
  r = c5->myFS->setAttributes(const_cast<char *>("/z"),2, 'y', 45);
  cout << (r>=0?"Correct, attributes are set": " Failure, check parameters") <<endl;
  r = c2->myFS->getAttributes(const_cast<char *>("/f"),2, 3); //Should fail (Invalid request)
  cout << "Requested attribute is " << r << (r>=0?"; this is correct": "; failure, check parameters") <<endl;
  r = c5->myFS->getAttributes(const_cast<char *>("/z"), 2, 1);
  cout << "Requested attribute is " << r << (r>=0?"; this is correct": "; failure, check parameters") <<endl;

  r = c3->myFS->setAttributes(const_cast<char *>("/o/o/o/a/l")2, 'y', -20); //Should fail (negative file rank)
  cout << (r>=0?"Correct, attributes are set": " Failure, check parameters") <<endl;
  r = c3->myFS->setAttributes(const_cast<char *>("/o/o/o/a/d"),2, 'r', 50);
  cout << (r>=0?"Correct, attributes are set": " Failure, check parameters") <<endl;
  r = c3->myFS->getAttributes(const_cast<char *>("/o/o/o/a/l"),2, 1);
  cout << "Requested attribute is " << r << (r>=0?"; this is correct": "; failure, check parameters") <<endl;
  r = c3->myFS->getAttributes(const_cast<char *>("o/o/o/a/d"), 2, 2);
  cout << "Requested attribute is " << r << (r>=0?"; this is correct": "; failure, check parameters") <<endl;
  
  r = c2->myFS->setAttributes(const_cast<char *>("/f"),2, 'r', 60);
  cout << (r>=0?"Correct, attributes are set": " Failure, check parameters") <<endl;
  r = c5->myFS->setAttributes(const_cast<char *>("/z"),2, 'b', 60);
  cout << (r>=0?"Correct, attributes are set": " Failure, check parameters") <<endl;
  r = c2->myFS->getAttributes(const_cast<char *>("/f"), 2, 1);
  cout << "Requested attribute is " << r << (r>=0?"; this is correct": "; failure, check parameters") <<endl;
  r = c5->myFS->getAttributes(const_cast<char *>("/z"), 2, 2);
  cout << "Requested attribute is " << r << (r>=0?"; this is correct": "; failure, check parameters") <<endl;

  r = c3->myFS->setAttributes(const_cast<char *>("/o/o/o/a/l"),2, 'y', 70);
  cout << (r>=0?"Correct, attributes are set": " Failure, check parameters") <<endl;
  r = c3->myFS->setAttributes(const_cast<char *>("/o/o/o/a/d"),2, 'b', 70);
  cout << (r>=0?"Correct, attributes are set": " Failure, check parameters") <<endl;
  r = c3->myFS->getAttributes(const_cast<char *>("/o/o/o/a/l"), 2, 2);
  cout << "Requested attribute is " << r << (r>=0?"; this is correct": "; failure, check parameters") <<endl;
  r = c3->myFS->getAttributes(const_cast<char *>("o/o/o/a/d"), 2, 2);
 cout << "Requested attribute is " << r << (r>=0?"; this is correct": "; failure, check parameters") <<endl;*/

  return 0;
}
