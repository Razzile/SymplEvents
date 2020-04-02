#pragma once 
#include <memory>
#include <mutex>
#include <string>
#include <unordered_set>
#include "delegate.h"


namespace symplevents {

struct EventBase {
  std::string identifier;

  explicit EventBase(const std::string& message) : identifier(message) {}

  virtual ~EventBase() = default;

  bool operator==(const EventBase* other) const {
    return identifier == other->identifier;
  }

  struct Hash {
    size_t operator()(const std::unique_ptr<EventBase>& ev) const {
      return std::hash<std::string>()(ev->identifier);
    }
  };
};

template <typename... Args>
struct Event : EventBase {
  using Trigger = std::function<void(Args...)>;

  Event(const std::string& name, const Trigger& func)
      : EventBase(name), trigger() {
    trigger.AddListener(func);
  }

  Delegate<Args...> trigger;
};

class EventManager {
 public:
  static EventManager& GlobalManager() {
    static EventManager manager;
    return manager;
  }

  EventManager() = default;

  /**
   * Subscribe a trigger to an event with the supplied identifier
   *
   * @param identifier
   *    The identifier of the event
   *
   * @param trigger
   *    The trigger to subscribe to the event
   */
  template <typename... Args>
  void Subscribe(const std::string& identifier,
                 const typename Event<Args...>::Trigger& trigger) {
    std::scoped_lock lock(send_mutex_, sub_mutex_);
    events_.insert(std::make_unique<Event<Args...>>(identifier, trigger));
  }

  /**
   * Trigger an event with the supplied identifier and pass the supplied arguments to subscribed listeners
   *
   * @param identifier
   *    The identifier of the event to trigger
   *
   * @param args
   *    The arguments to pass to subscribed triggers
   *
   * @throws std::runtime_error if the supplied arguments don't match those used when the event was subscribed
   */
  template <typename... Args>
  void Send(const std::string& identifier, Args... args) {
    std::scoped_lock lock(send_mutex_);
    for (const auto& event : events_) {
      if (event->identifier == identifier) {
        auto* ev = dynamic_cast<Event<Args...>*>(event.get());
        if (!ev) {
          throw std::runtime_error("Event triggered with incorrect arguments");
        } else {
          ev->trigger(args...);
        }
      }
    }
  }

 private:
  std::unordered_set<std::unique_ptr<EventBase>, EventBase::Hash> events_;
  std::mutex sub_mutex_;
  std::mutex send_mutex_;
};

}  // namespace symplevents

