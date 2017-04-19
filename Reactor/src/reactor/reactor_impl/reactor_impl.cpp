//
// Created by pairs on 4/18/17.
//

#include <reactor/event_handler.h>
#include "reactor_impl.h"
#include "event_demultiplexer/impl/epoll_demultiplexer.h"

namespace reactor
{

    ReactorImpl::ReactorImpl():event_io_ptr_(nullptr),event_timer_ptr_(nullptr)
    {
        event_io_ptr_ = std::make_shared<EpollDemultiplexer>();
        // todo add signal && time_heap
    }

    ReactorImpl::~ReactorImpl()
    {
    }

    int ReactorImpl::RegisterHandler(std::shared_ptr <IEventHandler> handler, event_t evt)
    {
        // just one thread not necessity but we may change to multithreading
        std::lock_guard<std::mutex> l(events_mutex_);
        event_handlers_[handler->get_handle()] = handler;
        return event_io_ptr_->RegisterEvent(handler->get_handle(), evt);
    }


    int ReactorImpl::UnRegisterHandler(std::shared_ptr<IEventHandler> handler)
    {
        std::lock_guard<std::mutex> l(events_mutex_);
        event_handlers_.erase(handler->get_handle());
        return event_io_ptr_->UnRegisterEvent(handler->get_handle());
    }

    void ReactorImpl::HandleEvents()
    {
        // TODO signal events
        // TODO timeout events
        event_io_ptr_->WaitEvent();
    }
}

