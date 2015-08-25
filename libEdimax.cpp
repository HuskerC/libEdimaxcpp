#include "libEdimax.h"
#include "tinyXML2/tinyxml2.h"

static std::string requestString = "<?xml version=\"1.0\" encoding=\"UTF8\"?> \
    <SMARTPLUG id=\"edimax\"> \
    <CMD id=\"get\"> \
    <NOW_POWER><Device.System.Power.NowCurrent></Device.System.Power.NowCurrent> \
    <Device.System.Power.NowPower></Device.System.Power.NowPower></NOW_POWER> \
    </CMD> \
    </SMARTPLUG>";

EdimaxDevice::EdimaxDevice(const std::string &hostname, int port,
                        const std::string &username, const std::string &password):
    m_Hostname(hostname),m_Port(port),m_Username(username),m_Password(password)
{
    m_Url = "http://"+hostname+":"+std::to_string(port)+"/smartplug.cgi";
    m_CurlHandle = curl_easy_init();
    if(!m_CurlHandle)
        throw std::exception();
    curl_easy_setopt(m_CurlHandle, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(m_CurlHandle, CURLOPT_USERNAME, m_Username.c_str());
    curl_easy_setopt(m_CurlHandle, CURLOPT_PASSWORD, m_Password.c_str());
    
}

EdimaxDevice::~EdimaxDevice()
{
    curl_easy_cleanup(m_CurlHandle);
}

EdimaxDevice::PowerInformation EdimaxDevice::getCurrentPowerUsage()
{
    m_ReceivedData.clear();
    curl_easy_setopt(m_CurlHandle, CURLOPT_WRITEFUNCTION, EdimaxDevice::write_callback);
    curl_easy_setopt(m_CurlHandle, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(m_CurlHandle, CURLOPT_URL,m_Url.c_str());
    curl_easy_setopt(m_CurlHandle, CURLOPT_POSTFIELDS, requestString.c_str());
    curl_easy_setopt(m_CurlHandle, CURLOPT_POSTFIELDSIZE, requestString.length());
    curl_easy_setopt(m_CurlHandle, CURLOPT_POST, 1);
    curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "User-Agent:EdiPlug/20150504 CFNetwork/711.3.18 Darwin/14.0.0");
    headers = curl_slist_append(headers, "Accept:*/*");
    headers = curl_slist_append(headers, "Content-Type:application/x-www-form-urlencoded");
    headers = curl_slist_append(headers, "Accept-encoding:gzip, deflate");
    headers = curl_slist_append(headers, "Connection:keep-alive");
    headers = curl_slist_append(headers, "Accept-Language: de-de");
 
    CURLcode res;
    res = curl_easy_perform(m_CurlHandle);
    if(res!=CURLE_OK)
    {
        curl_slist_free_all(headers);
        std::cout<<"Request Error:"<<res<<std::endl;
        return EdimaxDevice::PowerInformation();
    }
    
    curl_slist_free_all(headers);
    
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError err = doc.Parse(m_ReceivedData.c_str());
    if(err!=tinyxml2::XML_NO_ERROR)
    {
        doc.PrintError();
        throw std::exception();
    }
    tinyxml2::XMLElement *smatplug = doc.FirstChildElement("SMARTPLUG");
    if(smatplug)
    {
        tinyxml2::XMLElement *cmd = smatplug->FirstChildElement("CMD");
        if(cmd)
        {
            tinyxml2::XMLElement *now_power = cmd->FirstChildElement("NOW_POWER");
            if(now_power)
            {
                tinyxml2::XMLElement *power_now_current =
                now_power->FirstChildElement("Device.System.Power.NowCurrent");
                
                tinyxml2::XMLElement *power_now_power =
                now_power->FirstChildElement("Device.System.Power.NowPower");
                
                double watts,amps;
                err = power_now_power->QueryDoubleText(&watts);
                if(err==tinyxml2::XML_NO_ERROR)
                {
                    err = power_now_current->QueryDoubleText(&amps);
                    if(err==tinyxml2::XML_NO_ERROR)
                    {
                        return EdimaxDevice::PowerInformation(watts,amps);
                    }
                }
                
                
            }
        }
    }
    return EdimaxDevice::PowerInformation();
    
}

void EdimaxDevice::appendData(const std::string &data)
{
    //std::cout<<"Appending data:"<<data<<std::endl;
    m_ReceivedData.append(data);
}

size_t EdimaxDevice::write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    EdimaxDevice *device = (EdimaxDevice*)userdata;
    
    std::string data(ptr,nmemb);
    device->appendData(data);
    return size*nmemb;
    
}
