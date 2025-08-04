# Level Load

A few optimizations to speed up loading a level.

### Important disclaimers!

- Optimizations can be <cy>messed up by other mods</cy> that touch gameplay logic. This mod <cy>works best alone,</cy> but it shouldn't ever make loading times worse. Feel free to let me know of any mods that make this mod ineffective, but be warned that it <cy>may be unresolvable.</cy>

- I can't test on all <cy>platforms</cy> for all <cy>hardware;</cy> optimizations might <cy>lose their effectiveness</cy> compared to the environment I develop this mod in. Please let me know if this mod seems to not work for you!

## Optimizations

- Objects are created by first parsing a line of the level string as key-value pairs, then translating those pairs to properties of the object. Now, the parsing is done in a separate thread. <cy>This provides a decent speedup to most levels, though it can vary.</cy>

- That translation to actual properties repeats "Is this key present? Then parse the value as the appropriate type, else use a default" many times. Now, for known key-value pairs, this computation is done in the parsing thread, balancing work. The translation step can then just directly use the computed value. <cy>This provides some more speedup, like above.</cy>

### The below have niche impacts

- When playing an online level (as opposed to an editor or official level), the code that updates the circular progress bar also only runs one "create objects" step per frame. Now, this step is repeated for the length of a frame to avoid idle time. <cy>To be honest this is really only noticeable on high-end systems running at 60hz, playing low-end high object count levels.</cy>

- A piece of level setup code has quadratic-time performance... if you have a group with a ton of decorative objects that is targeted by a rotate or scale trigger. Now, this code runs in linearithmic time. <cy>The only level I am aware of that this noticeably impacts is LIMBO (this causes the long wait at the end of the progress bar), though it helps a lot!</cy>
