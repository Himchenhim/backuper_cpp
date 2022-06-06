#include <functional>
#include <queue>
#include <chrono>
#include <sys/time.h>

using std::function;


// for global variable  priority_queue<CEvent, std::vector<CEvent>, CEvent_Comp> event_queue
namespace scheduler
{
    class CEvent;

    // in object of this class will be saved function, which have to execute in exact time
    // functor

    // Comparator class for sorting priority queue -> push the most near events to the top
    class CEvent_Comp : public std::less<CEvent>
    {
        public:
        bool operator()(const CEvent &event1, const CEvent & event2) const;
    };

    class CEvent{
    public:
        typedef function<void()> callback_type;
        typedef std::chrono::time_point<std::chrono::system_clock> time_type;

        CEvent(const callback_type & call_back, const time_type & when);

        void operator() ()const;
        friend CEvent_Comp;

        time_type GetTimeToDo () const;

    private:
        callback_type callback_value;
        time_type time_to_do;
    };


    void add(const CEvent::callback_type &callback, const std::chrono::time_point<std::chrono::system_clock> &when);
    void add(const CEvent::callback_type &callback, const timeval &when);
    void add(const CEvent::callback_type &callback, const time_t &when);

    bool timer();

}