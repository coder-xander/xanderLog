#include <deque>
#include <memory>
#include <shared_mutex>
#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <fstream>
namespace xander
{
    // class DataTime
    // {
    // public:
    //     std::string year;
    //     std::string month_;
    //     std::string day;
    //     std::string hour_;
    //     std::string minute_;
    //     std::string second_;
    //     std::string mills_;
    //     std::string_view toString()
    //     {
    //         return year + "-" + month_ + "-" + day + " " + hour_ + ":" + minute_ + ":" + second_ + "." + mills_;
    //     }
    //     std::string_view toDateString()
    //     {
    //         return year + "-" + month_ + "-" + day;
    //     }
    //     std::string_view toTimeString()
    //     {
    //         return hour_ + ":" + minute_ + ":" + second_ + "." + mills_;
    //     }
    //
    // };
    //
    //
    // class DateTimeTool
    // {
    // public:
    //
    //     DataTime getCurrentDateTime() {
    //         auto now = std::chrono::system_clock::now();
    //         std::time_t now_t = std::chrono::system_clock::to_time_t(now);
    //
    //         std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    //
    //         std::tm* now_tm = std::localtime(&now_t);
    //
    //         DataTime datetime;
    //         datetime.year = std::to_string(1900 + now_tm->tm_year);
    //         datetime.month_ = std::to_string(1 + now_tm->tm_mon);
    //         datetime.day = std::to_string(now_tm->tm_mday);
    //         datetime.hour_ = std::to_string(now_tm->tm_hour);
    //         datetime.minute_ = std::to_string(now_tm->tm_min);
    //         datetime.second_ = std::to_string(now_tm->tm_sec);
    //         datetime.mills_ = std::to_string(ms.count());
    //
    //         return datetime;
    //     }
    // };
    ///@brief log
    ///@typedef Accuracy, the accuracy of log time
    class Log
    {
    public:
        enum Level
        {
            Trace = 0,
            Debug = 1,
            Info = 2,
            Warn = 3,
            Error = 4,
            Fatal = 5
        };

    protected:
        virtual  void build() = 0;
        Level level_;
        std::string_view content_;


    };
    template<typename Accuracy >
    class LogDetail : public std::enable_shared_from_this<LogDetail<Accuracy>>, public Log
    {
    private:
        //log base dir
        std::chrono::time_point<Accuracy> timePoint_;//日志发生的时间点
        std::string baseDir_;//main dir

        void build() override
        {
            //build log
            content_ = "log content";

        }
    };
    template<typename Accuracy >
    using LogDetailPtr = std::shared_ptr<LogDetail<Accuracy>>;
    using LogPtr = std::shared_ptr<Log>;
    //log appender, write log to file or console
    class Appender
    {
        using AppenderPtr = std::shared_ptr<Appender>;
    public:
        //some appender save .
        static inline std::vector<AppenderPtr> appenders_;
        std::string name_;
        std::shared_mutex logMutex_;
        std::deque<LogPtr> logs_;
        virtual void append(const LogPtr& log) = 0;
        //serialize log to your target
        virtual bool serialize(const LogPtr& log) = 0;
        virtual ~Appender() = default;
    };

    class FileAppender :public Appender
    {
    private:
        std::string logDir_;
    public:
        FileAppender()
        {
            logDir_ = "./log";

        }
        void append(const LogPtr& log) override
        {
            std::lock_guard<std::shared_mutex> lock(logMutex_);
            logs_.push_back(log);
            logs_.push_back({});
        }
        bool serialize(const LogPtr& log) override
        {
            return false;
        }
        ~FileAppender()override
        {


        }
    };

}