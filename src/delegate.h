#pragma once

#include <functional>
#include <mutex>

namespace symplevents {

template <typename... Args>
class Delegate {
public:
  using Listener = std::function<void(Args ...)>;

  virtual ~Delegate() = default;

  /**
   * Add a new listener function to the delegate
   *
   * @param listener
   *    The function to add
   */
  virtual void AddListener(const Listener& listener) {
    std::lock_guard<std::mutex> guard(lock_);
    listeners_.push_back(listener);
  }

  /**
   * Remove all listeners from delegate
   */
  virtual void RemoveAllListeners() {
    std::lock_guard<std::mutex> guard(lock_);
    listeners_.clear();
  }

  /**
   * Call all registered functions
   */
  virtual void operator()(Args ... args) {
    std::lock_guard<std::mutex> guard(lock_);
    for (auto& listener : listeners_) {
      listener(args...);
    }
  }

  /**
   * Returns the number of registered listeners in this delegate
   *
   * @return listener count 
   */
  int listeners() const {
    return listeners_.size();
  }

private:
  std::mutex lock_;
  std::vector<Listener> listeners_;
};

template <>
class Delegate<void> {
public:
  typedef std::function<void()> Listener;

  void AddListener(Listener const& listener) {
    std::lock_guard<std::mutex> guard(lock_);
    listeners_.push_back(listener);
  }

  void RemoveAllListeners() {
    std::lock_guard<std::mutex> guard(lock_);
    listeners_.clear();
  }

  void operator()() {
    std::lock_guard<std::mutex> guard(lock_);
    for (auto& listener : listeners_) {
      listener();
    }
  }

private:
  std::mutex lock_;
  std::vector<Listener> listeners_;
};

} // namespace symplevents
