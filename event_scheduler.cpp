#include "event_scheduler.hpp"

using namespace scheduler;
// for global variable  priority_queue<CEvent, std::vector<CEvent>, CEvent_Comp> event_queue

class CEvent_Comp;



scheduler::CEvent::CEvent(const callback_type & call_back, const time_type & when) : callback_value(call_back), time_to_do(when){}

void scheduler::CEvent::operator() ()const{
    callback_value();
}
scheduler::CEvent::time_type CEvent::GetTimeToDo () const{return time_to_do;}


// Comparator class for sorting priority queue -> push the most near events to the top
bool scheduler::CEvent_Comp::operator()(const CEvent &event1, const CEvent & event2) const
{
    return (event1.time_to_do < event2.time_to_do);
}

std::priority_queue<scheduler::CEvent, std::vector<scheduler::CEvent>, scheduler::CEvent_Comp> event_queue;


    void scheduler::add(const CEvent::callback_type &callback, const std::chrono::time_point<std::chrono::system_clock> &when)
    {
        event_queue.emplace(callback, when);
    }

    void scheduler::add(const CEvent::callback_type &callback, const timeval &when)
    {
        auto real_when = std::chrono::system_clock::from_time_t(when.tv_sec) +
                         std::chrono::microseconds(when.tv_usec);

        event_queue.emplace(callback, real_when);
    }

    void scheduler::add(const CEvent::callback_type &callback, const time_t &when)
    {
        auto real_when = std::chrono::system_clock::from_time_t(when);

        event_queue.emplace(callback, real_when);
    }

    bool scheduler::timer()
    {
        CEvent::time_type now = std::chrono::system_clock::now();

        if (!event_queue.empty() &&
               (event_queue.top().GetTimeToDo() < now))
        {
            event_queue.top()();
            event_queue.pop();
            return true;
        }
        return false;
    }
