# Level Load

A few optimizations to speed up loading a level.

- Objects are created by first parsing a line of the level string as key-value pairs, then translating those pairs to properties of the object. Now, the parsing is done in a separate thread. **This provides a decent speedup to most levels, though it can vary.**

- That translation to actual properties repeats "Is this key present? Then parse the value as the appropriate type, else use a default" many times. Now, for known key-value pairs, this computation is done in the parsing thread, balancing work. The translation step can then just directly use the computed value. **This provides some more speedup, like above.**

The below optimizations have very niche impacts:

- When playing an online level (as opposed to an editor or official level), the code that updates the circular progress bar also only runs one "create objects" step per frame. Now, this step is repeated for the length of a frame to avoid idle time. **This especially helps with low-end high object count levels, most noticeably so at 60hz, though also at higher framerates for higher-end systems.**

- A piece of level setup code has quadratic-time performance... if you have a group with a ton of decorative objects that is targeted by a rotate or scale trigger. Now, this code runs in linearithmic time. **The only level I am aware of that this noticeably impacts is LIMBO, though it helps a lot!**
