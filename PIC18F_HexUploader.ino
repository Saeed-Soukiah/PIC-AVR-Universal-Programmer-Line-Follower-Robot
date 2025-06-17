// Desclaimer: I Didn't Write THis Code//
///********************************************************************///
///          >>  PIC18F2XXX/4XXX Programmer  <<                      ///
///  Code Description: This program reads a HEX file for a PIC microcontroller
///       from the 18F series, then uploads it to the PIC using Arduino.      ///
///********************************************************************///
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <windows.h>
#include <math.h>
#include <fstream>
#include <string> 
using namespace std;

//------جميع الدوال المستخدمة في البرنامج-------
string              dec_to_hex(int);
int                 hex_to_dec(string);
void                gotoxy(int, int);
bool                readFile();
void                sendData();
bool                deviceSerch();
bool                eraseDevice();
bool                sendProgData();
bool                sendIdByte();
bool                sendEepromByte();
bool                sendConfigByte();
void                stopPrograming();
//-----------------------------------------------

DCB dcbSerialParams = { 0 }; // ضبط إعدادات التواصل مع الأردوينو
COMMTIMEOUTS timeouts = { 0 };

DWORD dwEventMask = 0x1;
HANDLE hComm;
DWORD nNumberOfBytesToRead = 1;
DWORD lpNumberOfBytesRead = 0;

DWORD dNoOFBytestoWrite;         // عدد البتات المراد كتابتها للمنفذ التسلسلي
DWORD dNoOfBytesWritten = 0;     // عدد البتات المكتوبة بالمنفذ التسلسلي
DWORD NoBytesRead = 0;

long int realAdress = 0;
int      adressU;
int      dataType = 0;
long int      adress=0;
long int      adresspoint=0;
int      byteNumb;
string   line = "aa";
string   val = "";
string   val0 = "";
string   val1 = "";
string   val2 = "";
int      point = 0;
int      point1 = 0;
int      point2 = 0;
int      point3 = 0;

char     progData[98304];
char     configData[14];
char     idData[8];
char     eepromData[1024];

char     resp[4];
bool     endfile = false;
bool     eepromE = false;
char     buffer[38];
bool     endbuffer = false;
int      i = 0;
int      j = 0;
int      p = 0;
int      blockselect = 0;
int      bytenum=0;
int      blockprog = 0;
int      block[3072];
int      blocknum=0;
int      blockpoint=0;

///************************************************************///
///                main() : الدالة                           ///
///             الخرج: الدالة الرئيسية                      ///
///************************************************************///
int main()
{  
    if(readFile())  sendData();
    cout << "press any key to excite ...";
    _getch(); 
}
///************************************************************///

///************************************************************///
///          readFile()  : الدالة                           ///
///          الخرج: قرائة الملف الستعشري                        ///
///************************************************************///
bool readFile()
{
    cout << "+---------------------------------------------------------------------------------+" << endl;
    cout << "|               PIC18F2XXX/4XXX PROGRAMMER (based on arduino UNO)                 |" << endl;
    cout << "|                           creation date : 15/10/2020                            |" << endl;
    cout << "+---------------------------------------------------------------------------------+" << endl;
    string filepath;
    cout << ">> type the HEX file path  " << endl;
    cout << ">> ";
    cin >> filepath;
    cout << endl;
    fstream myfile;
    myfile.open(filepath);

    if (myfile.is_open())
    {

        while (!endfile)
        {
            myfile >> line;

            val = line[7];
            val += line[8];
            dataType = hex_to_dec(val);

            switch (dataType)
            {
            case 0:
            {
                if (realAdress == 0x200000)
                { //------------------------------- id bitsالبتات التعريفية ----------------------------
                    for (i = 9; i <= 24; i = i + 2)
                    {
                        val = line[i];
                        val += line[i + 1];
                        idData[point1] = hex_to_dec(val);
                                           
                        point1++;
                    }
                }
                else if (realAdress == 0x300000)
                { //------------------------- configuration bitsبتات ضبط المتحكم -----------------
                    for (i = 9; i <= 36; i = i + 2)
                    {
                        val = line[i];
                        val += line[i + 1];                      
                        configData[point2] = hex_to_dec(val);
                        point2++;
                    }
                }
                else if (realAdress == 0xF00000)
                { // ----------------------------EEPROM memoryالكتابة في ذاكرة البيانات --------
                    eepromE = true;
                    val = line[1];
                    val += line[2];
                    byteNumb = hex_to_dec(val);                   
                   for (i = 0; i < byteNumb * 2; i = i + 2)
                    {
                        val = line[i + 9];
                        val += line[i + 10];
                        eepromData[point3] = hex_to_dec(val);
                        point3++;              
                    }
                }
                else 
                { // ---------------------------program memory الكتابة في ذاكرة البرمجة----------
                val = line[3];
                val += line[4];
                val += line[5];
                val += line[6];
                adress = hex_to_dec(val);
                adress += realAdress;
                j = 0;
                if ((adress - adresspoint) != 0)
                {
                    for (i = 0; i < (adress - adresspoint); i++)
                    {
                        progData[point] = 0xFF;
                        if (p == 31)
                        {
                            blockpoint++;
                            p = 0;
                        }
                        else p++;
                        j++;
                        point++;
                    }
                    adresspoint += j;
                }
                val = line[1];
                val += line[2];
                byteNumb = hex_to_dec(val);
                for (i = 0; i < byteNumb * 2; i = i + 2)
                {
                    val = line[i + 9];
                    val += line[i + 10];
                    progData[point] = hex_to_dec(val);
                    if (progData[point] != 0xff)
                    {
                        block[blockpoint] = 1;
                    }
                    if (p == 31)
                    {
                        blockpoint++;
                        p = 0;
                    }
                    else p++;
                    point++;
                    adresspoint++;
                    bytenum++;
                }
                }
            }
            break;
            case 1:
            {
                endfile = true;
            }
            break;
            case 3: 
            {
                val = line[9];
                val += line[10];
                val += line[11];
                val += line[12];
                adressU = hex_to_dec(val);
                realAdress = adressU;
                realAdress = realAdress << 16;     
            }
            break;
            }
        }
    }
    else {
        int msgboxID = MessageBox(NULL,L"Can't open  file",L"can't open ", MB_ICONSTOP | MB_OK );
        return false;
    }
    myfile.close();

       for (i = 0; i < 3072; i++)
    {
        if(block[i]) blocknum++;
    }
    return true;
}
///************************************************************///

///************************************************************///
///              sendData() :الدالة                         ///
///  الخرج: إرسال البيانات إلى الأردوينو لتكتب على المتحكم بيك  ///
///************************************************************///
void sendData()
{
     //-----------------------------COM PORT CONFIG ضبط منفذ التراسل التسلسلي----------
     dcbSerialParams.DCBlength = sizeof(dcbSerialParams); 
     dcbSerialParams.BaudRate = CBR_57600;  // Setting BaudRate = 57600
     dcbSerialParams.ByteSize = 8;         // Setting ByteSize = 8
     dcbSerialParams.StopBits = ONESTOPBIT;// Setting StopBits = 1
     dcbSerialParams.Parity = NOPARITY;  // Setting Parity = None
     timeouts.ReadIntervalTimeout = 50; // in milliseconds
     timeouts.ReadTotalTimeoutConstant = 50; // in milliseconds 50
     timeouts.ReadTotalTimeoutMultiplier = 50; // in milliseconds 50
     timeouts.WriteTotalTimeoutConstant = 50; // in milliseconds
     timeouts.WriteTotalTimeoutMultiplier = 10; // in milliseconds
     //--------------------------------------------------------------------------------------
     if (!deviceSerch()) return;
     if (!eraseDevice()) return;
      if (!sendProgData()) return;
      if (!sendIdByte()) return;
      if (!sendEepromByte()) return; 
      if (!sendConfigByte()) return;
      stopPrograming();
      CloseHandle(hComm); //Closing the Serial Port
    MessageBox(NULL, L"DONE PROGRAMING",L"DONE",MB_ICONEXCLAMATION | MB_OK );
}
///************************************************************///

///************************************************************///
///               deviceSerch() : الدالة                     ///
///          الخرج: البحث عن الأردوينو بأي منفذ تسلسلي متصلة بالحاسب           ///     
///************************************************************///
bool deviceSerch()
{
    bool portstat = false;
    int portnum = 0;
    cout << ">> searching for device ..." << endl<<endl;
    while (!portstat)
    {
        string gg = "\\\\.\\COM";
        gg = gg + to_string(portnum);
        wchar_t COMPORT[13];
        for (int m = 0; m <= gg.length(); m++)
        {
            COMPORT[m] = gg[m];
        }
        hComm = CreateFile(COMPORT, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        SetCommState(hComm, &dcbSerialParams);
        SetCommTimeouts(hComm, &timeouts);
        SetCommMask(hComm, EV_RXCHAR);
        if (hComm == INVALID_HANDLE_VALUE)
        {
            if (portnum == 255)
            {
                portnum = 0;
                cout << ">> no device was found  !!" << endl<<endl;
                return false;
            }
            else portnum++;
        }
        else
        {
            char buf[38];
            buf[0] = 'P';
            buf[1] = 'I';
            buf[2] = 'C';
            buf[3] = 'P';
            buf[4] = 'R';
            buf[5] = 'O';
            buf[6] = 'G';
            buf[7] = '!';
            for (int r = 8; r < 38; r++)
            {
                buf[r] = '0';
            }
            dNoOFBytestoWrite = 38;
            WriteFile(hComm, buf, 38, &dNoOfBytesWritten, NULL);
            SetCommMask(hComm, EV_RXCHAR);
            char empChar[6];
            ReadFile(hComm, &empChar, sizeof(empChar), &NoBytesRead, NULL);
            if (empChar[0] == 'Y' && empChar[1] == 'E' && empChar[2] == 'S' && empChar[3] == 'I' && empChar[4] == 'A' && empChar[5] == 'M')
            {
                cout << ">> PIC18F PROGRAMMER found on COMPORT : " << portnum << endl<<endl;
                portstat = true;
            }
            else
            {
                CloseHandle(hComm); //Closing the Serial Port
                portnum++;
            }
        }
    }
    return true;
}
///************************************************************///

///************************************************************///
///              eraseDevice()  : الدالة                ///
///             الخرج: مسح محتويات ذاكرة المتحكم بيك                  ///
///************************************************************///
bool eraseDevice()
{   
    char blo[] = "ERES";
    WriteFile(hComm, blo, 38, &dNoOfBytesWritten, NULL);
    SetCommMask(hComm, EV_RXCHAR);
    dwEventMask = 0x1;
    WaitCommEvent(hComm, &dwEventMask, NULL);
    ReadFile(hComm, &resp, sizeof(resp), &NoBytesRead, NULL);
    if (resp[0] == 'D' && resp[1] == 'O' && resp[2] == 'N' && resp[3] == 'E')
    {
        cout << ">> the PIC18 was erased successfully !! " << endl<<endl;
    }
    else
    {
        cout << ">> the device is not responding corectly" << endl;
        return false;
    }
    return true;
}
///************************************************************///

///************************************************************///
///               sendProgData() : الدالة                    ///
///              الخرج: إرسال البتات المعبرة عن الكود البرمجي  للمتحكم                   ///
///************************************************************///
bool sendProgData()
{
    buffer[0] = 'P';
    buffer[1] = 'R';
    buffer[2] = 'O';
     j = 0;
    int loc = 0;
    gotoxy(0, 14);
    cout << ">> programing the device : " << endl;
    gotoxy(82, 14);
    cout << "0%";
    float cx = 100.0000 / blocknum;
    float zez = 0;
    bool test = false;
    blockselect = 0;
    while (!endbuffer)
    {
        if (block[blockselect])
        {
            j = ((blockselect + 1) * 32) - 32;
            val = dec_to_hex(j);
            val0 = val[0];
            val0 += val[1];
            val1 = val[2];
            val1 += val[3];
            val2 = val[4];
            val2 += val[5];
            buffer[3] = hex_to_dec(val0);
            buffer[4] = hex_to_dec(val1);
            buffer[5] = hex_to_dec(val2);
            for (i = 6; i < 38; i++)
            {
                buffer[i] = progData[j];
                j++;
            }
            WriteFile(hComm, buffer, 38, 0, NULL);
            SetCommMask(hComm, EV_RXCHAR);
            dwEventMask = 0x1;
            WaitCommEvent(hComm, &dwEventMask, NULL);
            ReadFile(hComm, &resp, sizeof(resp), &NoBytesRead, NULL);
            if ((zez + cx) > 99)  zez = 100;
            else zez += cx;
            loc = (zez * 52) / 100;
            //-------الشريط المعبر عن مدى تقدم انتهاء البرمجة -----------
            gotoxy(80, 14);
            cout << (char)0x5D;
            gotoxy(26, 14);
            cout << (char)0x5B;
            for (int f = 0; f <= loc; f++)
            {
                cout << (char)0xFE;
            }
            gotoxy(82, 14);
            cout << (int)zez << "%" << endl<<endl;
            //-------------------------------
        }
        if (blockselect == 3071) endbuffer = true;
        else  blockselect++;
    }
    return true;
   }
///************************************************************///

///************************************************************///
///             sendIdByte() : الدالة                       ///
///        الخرج: أرسل البتات التعريفية للمستخدم إلى المتحكم       ///
///************************************************************///
bool sendIdByte()
{
    buffer[0] = 'U';
    buffer[1] = 'S';
    buffer[2] = 'I';
    buffer[3] = 'D';
    j = 0;
    for (i = 4; i < 38; i++)
    {
        if (j > 11)
        {
            buffer[i] = 0xFF;
        }
        else
        {
            buffer[i] = idData[j];
        }
        j++;
    }
    WriteFile(hComm, buffer, 38, 0, NULL);
    SetCommMask(hComm, EV_RXCHAR);
    dwEventMask = 0x1;
    WaitCommEvent(hComm, &dwEventMask, NULL);

    ReadFile(hComm, &resp, sizeof(resp), &NoBytesRead, NULL);
    cout << ">> id bits programed successfully !!" << endl<<endl;
    return true;
}
///************************************************************///

///************************************************************///
///            sendEepromByte() : الدالة                  ///
///     الخرج أرسل البيانات الخاصة بذاكرة البيانات ليتم كتابتها للمتحكم            ///
///************************************************************///
bool sendEepromByte()
{
    if (eepromE)
    {
        cout << ">> programing the EEPROM ..." << endl << endl;
        buffer[0] = 'E';
        buffer[1] = 'E';
        buffer[2] = 'P';
        buffer[3] = 'R';
        p = 0;
        endbuffer = false;
        while(!endbuffer) 
        {
            val = dec_to_hex(p);
            val1 = val[2];
            val1 += val[3];
            val2 = val[4];
            val2 += val[5];
            buffer[4] = hex_to_dec(val1);
            buffer[5] = hex_to_dec(val2);
            for (i = 6; i < 38; i++)
            {
                if (p+6 > point3)
                {
                    buffer[i] = 0xFF;
                    endbuffer = true;
                }
                else
                {
                    buffer[i] = eepromData[p];
                }
                p++;
            }
            WriteFile(hComm, buffer, 38, 0, NULL);
            SetCommMask(hComm, EV_RXCHAR);
            dwEventMask = 0x1;
            WaitCommEvent(hComm, &dwEventMask, NULL);

            ReadFile(hComm, &resp, sizeof(resp), &NoBytesRead, NULL);
        }
        cout << ">> eeprom bits programed successfully !!" << endl<<endl;
    }
    return true;
}
///************************************************************///

///************************************************************///
///             sendConfigByte() : الدالة                   ///
///    الخرج: أرسل بتات ضبط قيم الفيوزات للأردوينو ليتم كتابتها للمتحكم بيك      ///
///************************************************************///
bool sendConfigByte()
{
    buffer[0] = 'C';
    buffer[1] = 'O';
    buffer[2] = 'N';
    buffer[3] = 'F';
    buffer[4] = 'I';
    buffer[5] = 'G';
    j = 0;
    for (i = 6; i < 38; i++)
    {
        if (j > 19)
        {
            buffer[i] = 0xFF;
        }
        else
        {
            buffer[i] = configData[j];
        }
        j++;
    }
    WriteFile(hComm, buffer, 38, 0, NULL);
    SetCommMask(hComm, EV_RXCHAR);
    dwEventMask = 0x1;
    WaitCommEvent(hComm, &dwEventMask, NULL);
    ReadFile(hComm, &resp, sizeof(resp), &NoBytesRead, NULL);
    cout << ">> config bits programed successfully !!" << endl<<endl;
    return true;
 }
///************************************************************///

///************************************************************///
///             stopPrograming() : الدالة                   ///
///   الخرج: أوقف عملية برمجة المتحكم ///
///************************************************************///
void stopPrograming()
{
    WriteFile(hComm, "OF", 38, 0, NULL);
    SetCommMask(hComm, EV_RXCHAR);
    dwEventMask = 0x1;
    WaitCommEvent(hComm, &dwEventMask, NULL);
    ReadFile(hComm, &resp, sizeof(resp), &NoBytesRead, NULL);
    cout << ">> the device was programed successfully !!" << endl<<endl;
}
///************************************************************///

///***********************************************************///
///             hex_to_dec(string ) : الدالة                ///
///             الخرج: تحويل قيمة ستعشرية إلى عشرية             ///
///***********************************************************///
int hex_to_dec(string str1)
{
    string x;
    int num0[100];
    long int num11 = 0;
    int j11 = 0, j33;
    int ji = 0;
    while (str1[ji] != NULL)
    {
        switch (str1[ji])
        {
        case 'A':
            num0[ji] = 10;
            break;
        case 'B':
            num0[ji] = 11;
            break;
        case 'C':
            num0[ji] = 12;
            break;
        case 'D':
            num0[ji] = 13;
            break;
        case 'E':
            num0[ji] = 14;
            break;
        case 'F':
            num0[ji] = 15;
            break;
        case '0':
            num0[ji] = 0;
            break;
        case '1':
            num0[ji] = 1;
            break;
        case '2':
            num0[ji] = 2;
            break;
        case '3':
            num0[ji] = 3;
            break;
        case '4':
            num0[ji] = 4;
            break;
        case '5':
            num0[ji] = 5;
            break;
        case '6':
            num0[ji] = 6;
            break;
        case '7':
            num0[ji] = 7;
            break;
        case '8':
            num0[ji] = 8;
            break;
        case '9':
            num0[ji] = 9;
            break;
        default:
            x = str1[ji];
            break;
        }
        j11 = ji;
        ji++;
    }
    ji = 0;
    j33 = 0;
    for (int j22 = j11; j22 >= 0; j22--)
    {
        num11 = num11 + (num0[j22] * pow(16, j33));
        j33++;
    }
    j33 = 0;
    return num11;
}
///***********************************************************///

///***********************************************************///
///           dec_to_hex(int )  : الدالة                   ///
///        الخرج: تحويل عدد صحيح إلى ستعشري         ///
///***********************************************************///
string dec_to_hex(int num4)
{
    int num5[100];
    int ji;
    for (ji = 0; ji < 100; ji++)
    {
        num5[ji] = 0;
    }
    string str4 = "";
    if (num4 == 0)
    {
        return "000000";
    }
    else
    {
        for (ji = 0; num4 > 0; ji++)
        {
            num5[ji] = num4 % 16;
            num4 = num4 / 16;
        }
        for (ji = 5; ji >= 0; ji--)
        {
            switch (num5[ji])
            {
            case 0:
                str4 = str4 + "0";
                break;
            case 1:
                str4 = str4 + "1";
                break;
            case 2:
                str4 = str4 + "2";
                break;
            case 3:
                str4 = str4 + "3";
                break;
            case 4:
                str4 = str4 + "4";
                break;
            case 5:
                str4 = str4 + "5";
                break;
            case 6:
                str4 = str4 + "6";
                break;
            case 7:
                str4 = str4 + "7";
                break;
            case 8:
                str4 = str4 + "8";
                break;
            case 9:
                str4 = str4 + "9";
                break;
            case 10:
                str4 = str4 + "A";
                break;
            case 11:
                str4 = str4 + "B";
                break;
            case 12:
                str4 = str4 + "C";
                break;
            case 13:
                str4 = str4 + "D";
                break;
            case 14:
                str4 = str4 + "E";
                break;
            case 15:
                str4 = str4 + "F";
                break;
            default:
                str4 = str4 + "0";
                break;
            }
        }
        return str4;
    }
}
///***********************************************************///

///***********************************************************///
///             gotoxy(int,int) : الدالة                   ///
///     الخرج: تغيير مكان مؤشر الكتابة             ///
///***********************************************************///
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
///************************************************************///
                return 1
        return 0
