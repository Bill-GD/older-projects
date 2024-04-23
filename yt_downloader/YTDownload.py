from pytube import YouTube
import os

url = input("Input YouTube link here: ")
print('Fetching video...')

link = YouTube(url)
audio = link.streams.filter(only_audio = True).first()

print("Downloading...")
path = audio.download()

print('Done')
base, ex = os.path.splitext(path)
newName = base + '.mp3'
os.rename(path, newName)

print('Video saved at: ', newName)

# test: https://www.youtube.com/watch?v=UcgcK79dF50