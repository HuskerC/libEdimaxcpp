#include <iostream>
#include <string>

#include <curl/curl.h>

class EdimaxDevice
{
public:
    struct PowerInformation{
        PowerInformation(double lwatts=-1.,double lamps=-1.):watts(lwatts),amps(lamps)
        {
            
        }
        
        double watts;
        double amps;
    };
    EdimaxDevice(const std::string &hostname,int port=10000,
                 const std::string &username="admin",
                 const std::string &password="1234");
    ~EdimaxDevice();

    PowerInformation getCurrentPowerUsage();
    
private:
    static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
    void appendData(const std::string& data);
    
    
    std::string m_Hostname,m_Username,m_Password;
    int m_Port;
    std::string m_Url;
    CURL *m_CurlHandle;
    std::string m_ReceivedData;
    
};