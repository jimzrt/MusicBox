"""
export GOOGLE_APPLICATION_CREDENTIALS="/home/james/My Project-26c36088605d.json"
"""

from google.cloud import texttospeech
import time
import os.path


text_arr = ['Hallo Meister!', 'Musikbox initialisiert!', 'Initialisiere Musikbox!',
            'Leere Spielfigur erkannt! Wähle einen Ordner mit den Tasten 1 und 2 aus und bestätige mit Taste 3.', 'Spielfigur entfernt', 'Spielfigur erkannt', 'Spielfigur gelöscht', 'Spielfigur löschen. Bestätige Löschen mit Taste 3 oder entferne die Figur um den Vorgang abzubrechen!', "Löschen der Figur fehlgeschlagen, bitte erneut versuchen!", "Speichern der Figur fehlgeschlagen, bitte erneut versuchen!", "Spielfigur erfolgreich gespeichert!", "Ordner", "Track"]

# Instantiates a client
client = texttospeech.TextToSpeechClient()

# Build the voice request, select the language code ("en-US") and the ssml
# voice gender ("neutral")
voice = texttospeech.types.VoiceSelectionParams(
    language_code='de-DE',
    name='de-DE-Wavenet-C',
    ssml_gender=texttospeech.enums.SsmlVoiceGender.NEUTRAL)

# Select the type of audio file you want returned
audio_config = texttospeech.types.AudioConfig(
    audio_encoding=texttospeech.enums.AudioEncoding.MP3)

# Perform the text-to-speech request on the text input with the selected
# voice parameters and audio file type

for i in range(1, 256):
    filename = 'mp3/%04d.mp3' % i
    if os.path.isfile(filename):
        print('skipping %s' % filename)
        continue
    text = str(i)
    response = client.synthesize_speech(
        texttospeech.types.SynthesisInput(text=text), voice, audio_config)
    with open(filename, 'wb') as out:
        out.write(response.audio_content)
        print('%d/255 - Audio content written to file' % i)
        time.sleep(1)

i += 1
for j, text in enumerate(text_arr):
    filename = 'mp3/%04d.mp3' % (i+j)
    if os.path.isfile(filename):
        print('skipping %s' % filename)
        continue
    response = client.synthesize_speech(
        texttospeech.types.SynthesisInput(text=text), voice, audio_config)
    with open(filename, 'wb') as out:
        out.write(response.audio_content)
        print('%d/%d - Audio content written to file' %
              (i + j, 255 + len(text_arr)))
        time.sleep(1)
