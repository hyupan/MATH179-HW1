  #include <math.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <iostream>
  #include <fstream>
  using namespace std;

  //FUNCTION HEADERS HERE
  //   FUNCTIONS FOR READING AND WRITING
  double **readpicture(ifstream &infile, int &width, int &height);
  char readchar(ifstream& infile, int &num);
  int readint(ifstream& infile, int numbytes, int &num);
  int char2int(char thechar);
  int char2int(char *thechar, int numbytes);
  void readrgb(ifstream& infile, double *rgb, int &num);
  void writepicture(ofstream& outfile, double **f, int width, int height);
  char int2char(unsigned long theint);
  void int2char(char *thechar, int theint);
  void writechar(ofstream &outfile, char thechar);
  void writeint(ofstream &outfile, int theint, int numbytes);
  void writergb(ofstream& outfile, double r, double g, double b);
  //   FUNCTIONS FOR CREATING AND REMOVING MEMORY
  double **matrix(int width, int height);
  void free_matrix(double **f, int width, int height);
  //   YOUR FUNCTIONS HERE
//  double changeIntensity(double c);
//  void roundNumThree(double **f, int width, int height);
//  void eightIntensity(double **f, int width, int height);
//  void roundNumFour(double **f, int width, int height);
    void roundNumFive(double **f, int width, int height);


  //MAIN BODY HERE
  int main()
  {
     int width;
     int height;
     double **f;
     double x, y, h;
     int i, j;
    //  int countZeros;
    //  int countOnes;

  // if reading in picture
     ifstream infile("deadflowers.bmp", ios::in | ios::binary);
     f = readpicture(infile,width,height);
     infile.close();
  // if creating picture
  //   width = 100;
  //   height = 100;
  //   f = matrix(width,height);
    //
    // countZeros = 0; countOnes = 0;
    // for(i = 0; i < width; i++) {
    //   for(j = 0; j < height; j++) {
    //     f[i][j] = changeIntensity(f[i][j]);
    //     if(f[i][j] == 1) {
    //       ++countOnes;
    //     } else {
    //       ++countZeros;
    //     }
    //   }
    // }
    //roundNumThree(f, width, height);
  //  roundNumFour(f, width, height);
  //  eightIntensity(f, width, height);
      roundNumFive(f, width, height);

  // printf("%s%d\n%s%d\n", "number of pixels intensity 0: ",  countZeros, "number of pixels intensity 1: ", countOnes);
  // writing picture
     ofstream outfile("result.bmp", ios::out | ios::binary);
     writepicture(outfile,f,width,height);
     outfile.close();

     free_matrix(f,width,height);
  }

  //FUNCTION CONTENT HERE

  //**************************************************************************
  //FUNCTIONS FOR READING AND WRITING
  //**************************************************************************
  double **readpicture(ifstream &infile, int &width, int &height)
  {
     int i, j, k;
     char junk, theformat[2];
     double rgb[3], **f;
     int num = 0;
     int upsidedown = 0;
     cout << "reading picture" << endl;

     for (i = 0; i < 2; i++)
        theformat[i] = readchar(infile,num);
     int filesize = readint(infile,4,num);
     cout << "   numbytes = " << filesize << endl;
     for (i = 0; i < 4; i++)
        junk = readchar(infile,num);
     int offset = readint(infile,4,num);
     int headerstart = num;
     int headersize = readint(infile,4,num);
     if (headersize == 12)
     {
        width = readint(infile,2,num);
        height = readint(infile,2,num);
     }
     else
     {
        width = readint(infile,4,num);
        height = readint(infile,4,num);
     }
     if (height < 0)
     {
        height = abs(height);
        upsidedown = 1;
     }
     cout << "   width = " << width << endl;
     cout << "   height = " << height << endl;
     int numcolorpanes = readint(infile,2,num);
     int bitsperpixel = readint(infile,2,num);
     if (bitsperpixel != 24)
     {
        cout << "ERROR: this program is only built for 1 byte per rgb, not a total of "
             << bitsperpixel << " bits" << endl;
        exit(1);
     }
     for (i = num-headerstart; i < headersize; i++)
        junk = readchar(infile,num);

     f = matrix(width,height);

     if (upsidedown)
        for (j = height-1; j >= 0; j--)
        {
           for (i = 0; i < width; i++)
           {
              readrgb(infile,rgb,num);
              f[i][j] = 0.0;
              for (k = 0; k < 3; k++)
                 f[i][j] += rgb[k]*rgb[k];
              f[i][j] = sqrt(f[i][j])/sqrt(3.0);
           }
           for (i = 0; i < (4-(3*width)%4)%4; i++)
              junk = readchar(infile,num);
        }
     else
        for (j = 0; j < height; j++)
        {
           for (i = 0; i < width; i++)
           {
              readrgb(infile,rgb,num);
              f[i][j] = 0.0;
              for (k = 0; k < 3; k++)
                 f[i][j] += rgb[k]*rgb[k];
              f[i][j] = sqrt(f[i][j])/sqrt(3.0);
           }
           for (i = 0; i < (4-(3*width)%4)%4; i++)
              junk = readchar(infile,num);
        }

     return f;
  }

  int readint(ifstream& infile, int numbytes, int &num)
  {
     char *temp;
     int i;
     int theint;

     temp = new char[numbytes];

     for (i = 0; i < numbytes; i++)
        infile.read(reinterpret_cast<char *>(&(temp[i])),sizeof(char));
     theint = char2int(temp,numbytes);

     delete[] temp;
     num += numbytes;

     return theint;
  }

  int char2int(char thechar)
  {
     int i, theint, imask;
     char cmask;

     cmask = 1;
     imask = 1;
     theint = 0;
     for (i = 0; i < 8; i++)
     {
        if (thechar & cmask)
        {
           theint += imask;
        }
        cmask = cmask << 1;
        imask *= 2;
     }

     return theint;
  }

  int char2int(char *thechar, int numbytes)
  {
     int i;
     int theint, power;

     power = 1;
     theint = 0;
     for (i = 0; i < numbytes; i++)
     {
        theint += char2int(thechar[i])*power;
        power = power*256;
     }

     return theint;
  }

  char readchar(ifstream& infile, int &num)
  {
     char thechar;

     infile.read(reinterpret_cast<char *>(&thechar),sizeof(char));
     num++;

     return thechar;
  }

  void readrgb(ifstream& infile, double *rgb, int &num)
  {
     char ctemp;
     int i, itemp, ijunk;

     for (i = 0; i < 3; i++)
     {
  //      ctemp = readchar(infile);
  //      itemp = char2int(ctemp);
        itemp = readint(infile,1,ijunk);
        rgb[i] = static_cast<double>(itemp)/255.0;
        num++;
     }
  }

  void writepicture(ofstream& outfile, double **f, int width, int height)
  {
     int numbytes;
     int i, j;
     cout << "writing picture" << endl;

     writechar(outfile,66);
     writechar(outfile,77);

  // number of bytes
     numbytes = 54+height*(3*width+(4-(3*width)%4)%4);
     cout << "   numbytes = " << numbytes << endl;
     writeint(outfile,numbytes,4);

     writeint(outfile,0,2);
     writeint(outfile,0,2);
     writeint(outfile,54,4);
     writeint(outfile,40,4);

  // width
     cout << "   width = " << width << endl;
     writeint(outfile,width,4);
  // height
     cout << "   height = " << height << endl;
     writeint(outfile,height,4);

     writeint(outfile,1,2);
     writeint(outfile,24,2);
     writeint(outfile,0,4);
     writeint(outfile,16,4);
     writeint(outfile,2835,4);
     writeint(outfile,2835,4);
     writeint(outfile,0,4);
     writeint(outfile,0,4);

     for (j = 0; j < height; j++)
     {
        for (i = 0; i < width; i++)
           writergb(outfile,f[i][j],f[i][j],f[i][j]);
        for (i = 0; i < (4-(3*width)%4)%4; i++)
           writechar(outfile,0);
     }
  }

  char int2char(unsigned long theint)
  {
     char thechar, mask;
     int i;

     mask = 1;
     thechar = 0;
     for (i = 0; i < 8; i++)
     {
        if (theint%2)
           thechar += mask;
        theint /= 2;
        mask = mask << 1;
     }

     return thechar;
  }

  void int2char(char *thechar, int theint, int numbytes)
  {
     int temp = theint;
     int i;

     for (i = 0; i < numbytes; i++)
     {
        thechar[i] = int2char(temp%256);
        temp = temp/256;
     }
  }

  void writechar(ofstream& outfile, char thechar)
  {
     outfile.write(reinterpret_cast<char *>(&thechar),sizeof(char));
  }

  void writeint(ofstream &outfile, int theint, int numbytes)
  {
     char *temp = new char[numbytes];
     int i;

     int2char(temp,theint,numbytes);
     for (i = 0; i < numbytes; i++)
        outfile.write(reinterpret_cast<char *>(&(temp[i])),sizeof(char));

     delete[] temp;
  }

  void writergb(ofstream& outfile, double r, double g, double b)
  {
     int i, irgb;
     char temp;
     char a;
     double rgb[3];

     rgb[0] = r;
     rgb[1] = g;
     rgb[2] = b;
     for (i = 0; i < 3; i++)
     {
        irgb = static_cast<int>(floor(255.0*rgb[i]+0.5));
        if (irgb < 0)
           irgb = 0;
        else if (irgb > 255)
           irgb = 255;

        temp = int2char(irgb);
        outfile.write(reinterpret_cast<char *>(&temp),sizeof(char));
     }
  }

  //**************************************************************************
  //FUNCTIONS FOR CREATING AND REMOVING MEMORY
  //**************************************************************************

  double **matrix(int width, int height)
  {
     double **f;
     int i;

     f = new double*[width];
     for (i = 0; i < width; i++)
        f[i] = new double[height];

     return f;
  }

  void free_matrix(double **f, int width, int height)
  {
     int i;

     for (i = 0; i < width; i++)
        delete f[i];
     delete f;
  }

  //**************************************************************************
  //YOUR FUNCTIONS HERE
  //**************************************************************************
  //double changeIntensity(double c) {
  //  return (double)round(c);
//  }
  // void eightIntensity(double **f, int width, int height) {
  //     int i, j, count1, count2, count3, count4, count5, count6, count7, count8;
  //     count1 = 0;
  //     count2 = 0;
  //     count3 = 0;
  //     count4 = 0;
  //     count5 = 0;
  //     count6 = 0;
  //     count7 = 0;
  //     count8 = 0;
  //
  //     for(i = 0; i < height; i++) {
  //       for(j = 0; j < width; j++) {
  //         if(f[i][j] >= 0.0 && f[i][j] < 0.175) {
  //           f[i][j] = 0;
  //           count1++;
  //         } else if(f[j][i] >= 0.175 && f[j][i] < 0.2){
  //           f[i][j] = 0.175;
  //           count2++;
  //         } else if(f[j][i] >= 0.2 && f[j][i] < 0.225) {
  //           f[j][i] = 0.2;
  //           count3++;
  //         } else if(f[j][i] >= 0.225 && f[j][i] < 0.275) {
  //           f[j][i] = 0.225;
  //           count4++;
  //         } else if(f[j][i] >= 0.275 && f[j][i] < 0.3) {
  //           f[j][i] = 0.275;
  //           count5++;
  //         } else if(f[j][i] >= 0.3 && f[j][i] < 0.325) {
  //           f[j][i] = 0.3;
  //           count6++;
  //         } else if(f[j][i] >= 0.325 && f[j][i] < 0.5) {
  //           f[j][i] = 0.325;
  //           count7++;
  //         } else if(f[j][i] >= 0.5 && f[j][i] <= 1.0) {
  //           f[j][i] = 1;
  //           count8++;
  //         }
  //       }
  //     }
  //     printf("%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n", count1, count2, count3, count4, count5, count6, count7, count8);
  // }

  // void roundNumThree(double **f, int width, int height) {
  //   int i, j;
  //   for(j = 0; j < width; j++) {
  //     for(i = 0; i < height; i++) {
  //       f[j][i] = (0.45*f[j][i]);
  //     }
  //   }
  // }
  // void roundNumFour(double **f, int width, int height) {
  //   int i, j;
  //   double min, max;
  //   min = 1.0;
  //   max = 0.0;
  //   for(j = 0; j < width; j++) {
  //     for(i = 0; i < height; i++) {
  //       if(f[j][i] < min) {
  //         min = f[j][i];
  //       } else if(f[j][i] > max) {
  //         max = f[j][i];
  //       }
  //     }
  //   }
  //   printf("%f\n%f\n", min, max);
  //   for(j = 0; j < width; j++) {
  //     for(i = 0; i < height; i++) {
  //     //  printf("%f\n", f[i][j]);
  //       f[j][i] = (f[j][i]-min)/(max-min);
  //       //printf("%f\n", f[i][j]);
  //     }
  //   }
  // }

  void roundNumFive(double **f, int width, int height) {
    int i, j;
    double min, max;
    min = f[0][0];
    max = f[0][0];
    for(i = 0; i < width; i++) {
      for(j = 0; j < height; j++) {
        if(f[i][j] < min) {
          min = f[i][j];
        } else if (f[i][j] > max) {
          max = f[i][j];
        }
      }
    }
    printf("%f\n%f\n", min, max);
    for(j = 0; j < width; j++) {
        for(i = 0; i < height; i++) {
        //  printf("%f\n", f[i][j]);
            f[j][i] = (f[j][i]-min)/(max-min);
          //printf("%f\n", f[i][j]);
        }
      }
  }
