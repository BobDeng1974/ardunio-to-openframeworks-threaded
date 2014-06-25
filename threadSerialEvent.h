#ifndef _THREADED_OBJECT
#define _THREADED_OBJECT

#include "ofMain.h"




class threadSerialEvent : public ofThread{
    
public:
    
    
    int count;  // threaded fucntions that share data need to use lock (mutex)

    
    
    ofSerial serial;
    
    bool		bSendSerialMessage;			// a flag for sending serial
    char		bytesRead[3];				// data from serial, we will be trying to read 3
    char		bytesReadString[4];			// a string needs a null terminator, so we need 3 + 1 bytes
    int			nBytesRead;					// how much did we read?
    int			nTimesRead;					// how many times did we read?
    float		readTime;
    
    string fullMessage;
    int interMessage;
    string Sel;
    int Sensor;      // HOLDS PULSE SENSOR DATA FROM ARDUINO
    int IBI;         // HOLDS TIME BETWEN HEARTBEATS FROM ARDUINO
    float mph;         // HOLDS HEART RATE VALUE FROM ARDUINO
    bool heart;
    bool onOff;
    
    
    
    threadSerialEvent(){
        count = 0;
    }
    
    void open(int baud){
        serial.listDevices();
        vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
       // int baud = 115200;
        serial.setup(0, baud);
        interMessage = 0;
        startThread(true, false);   // blocking, verbose
    }
    
    void stop(){
        stopThread();
    }
    
    //--------------------------
    void threadedFunction(){
        
        while( isThreadRunning() != 0 ){
            if( lock() ){
             //   count++;
//ofSleepMillis(1);
                unsigned char inChar;
                string inString;
                string tempString;
                //serial.writeByte(inChar);
                //   unsigned char inChary = ofToChar("");
                if (serial.available() > 0) {
                    //   serial.
                    
                    inChar = serial.readByte();
                    inString += inChar;
                    //   cout << inChar << " why  "<< serial.available() << endl;
                    string findBreak = ofToString(inChar);
                    tempString += ofToString(inChar);
                    
                    //        if (findBreak == "\n"){
                    //            cout << "end of line " << " " << tempString  << " " << inChar << endl;
                    //        }
                }
                
                if(inString != ""){
                    
                    vector<string> dataFromArdunio = ofSplitString(inString,",");
                    
                    for(int a = 0;a<dataFromArdunio.size();a++){
                        
                        if(dataFromArdunio[a] == "\n"){
             
                            if(Sel == "O"){
                                onOff = true;
                                cout << " hi " << endl;
                            }
                            
                
                            
                            if(Sel == "S"){
                                
                                Sensor = ofToInt(fullMessage);
                            //    cout << " Sensor" <<  " " << Sensor  << endl;
                                //   cout << " ssssss " << " " << Sensor << " " << fullMessage << endl;
                            }
                            
                            if(Sel == "M"){
                                mph = ofToInt(fullMessage);
                          //      cout << " BPM " <<  " " << BPM  << endl;
                                heart = true;
                                //cout << Sel << " j finnish " <<  endl;
                                
                                
                            }
                            if(Sel == "Q"){
                                IBI = ofToInt(fullMessage);
                              //  cout << " IBI " <<  " " << IBI  << endl;
                            }
                            //    cout << fullMessage  << endl;
                            
                            fullMessage = "";
                            interMessage = 0;
                            Sel = "";
                            serial.drain();
                        }
                        else{
                            //  cout << interMessage <<  " " << dataFromArdunio[a] << endl;
                            interMessage ++;
                            
                            // cout << dataFromArdunio[a] << " " << interMessage << endl;
                            if(interMessage == 1){
                                Sel = dataFromArdunio[a];
                            }else{
                                fullMessage += dataFromArdunio[a];
                            }
                        }
                        
                        
                    }
                    
                    
                    
                    
                }
                
                unlock();

           }
        }
    }
    
    //--------------------------
    void draw(){
        
        string str = "I am a slowly increasing thread. \nmy current count is: ";
        
        if( lock() ){
       //     str += ofToString(count);
            unlock();
        }else{
            str = "can't lock!\neither an error\nor the thread has stopped";
        }
       // ofDrawBitmapString(str, 50, 56);
    }
    
    
    
};

#endif