# SymplEvents

SymplEvents is a lightweight library providing event registration/triggering.

This project was made by me during COVID-19 quarantine and is probably janky. It is not recommended you use this code in your project.

### Example

#### Events
Using the EventManager class you can listen for, or trigger events
```cpp
using symplevents::EventManager;

EventManager& manager = EventManager::GlobalManager();

// subscribe to 'hello' event taking a string and an int with a callback
manager.Subscribe<std::string, int>(
    "hello", [](const std::string& animal, int count) -> void
    {
        std::cout << "The quick brown " << animal << " jumped over the lazy dog " << count << " times" << std::endl;
    });


// trigger the event 

manager.Send<std::string, int>("hello", "Fox", 10);
```

### Delegates
Using the delegate class you can impement your own event handler within your own classes.
Below is a hypothetical use case for delegates:

```cpp
using symplevents::Delegate;
class Button {
 public:
    // constructors and methods etc...

    enum class EventType {
        EventType1,
        EventType2
	};

    Delegate<EventType> on_click;
    Delegate<EventType> on_right_click;
};

// ...

Button &button = window.MakeButton(0, 0, "Click me!");

button.on_click.AddListener([](EventType event) {
    // do something with event
});
```