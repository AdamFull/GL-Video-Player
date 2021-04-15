#ifndef __SE_LOGTRACE__
#define __SE_LOGTRACE__

#include <iostream>
#include <string>
#include <ctime>

/*****************************************************************************************/
class LogTrace {
public:
    virtual ~LogTrace() {}
    static std::string getTime(){
        time_t now = time(0);
        struct tm *timeinfo;
        char buffer[80];
        timeinfo = localtime(&now);
        strftime(buffer,sizeof(buffer),"[%d-%m-%Y %H:%M:%S]: ",timeinfo);
        return std::string(buffer);
    }
    virtual void Trace(std::string) const = 0;
};

/*****************************************************************************************/
class ConcreteConsoleLog : public LogTrace {
public:
    void Trace(std::string toSend) const override {
        std::string timeString = getTime();
        std::cout << timeString.append(toSend) << std::endl;
    }
};

/*****************************************************************************************/
class ConcreteFileLog : public LogTrace {
public:
    void Trace(std::string toSend) const override {
        //Open file and write
    }
};

/*****************************************************************************************/
class CreateLogTracer {
public:
    virtual ~CreateLogTracer(){};
    virtual LogTrace* FactoryMethod() const = 0;

    void SendLog(std::string traceMsg) const {
        LogTrace* logtype = this->FactoryMethod();
        logtype->Trace(traceMsg);
        delete logtype;
    }
};

/*****************************************************************************************/
class GetConsoleLog : public CreateLogTracer {
public:
    LogTrace* FactoryMethod() const override {
        return new ConcreteConsoleLog();
    }
};

/*****************************************************************************************/
class GetFileLog : public CreateLogTracer {
public:
    LogTrace* FactoryMethod() const override {
        return new ConcreteFileLog();
    }
};

#endif