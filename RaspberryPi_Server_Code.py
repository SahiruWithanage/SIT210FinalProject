from fastapi import FastAPI, Response
from pydub import AudioSegment
import io

app = FastAPI()

@app.get("/audio")
async def stream_audio():
    # Load audio file
    audio_file_path = "/home/s223047862/wavtest.wav"
    audio = AudioSegment.from_mp3(audio_file_path)

    # Convert audio to WAV format with desired specifications
    wav_buffer = io.BytesIO()
    audio.export(wav_buffer, format="wav", bitrate="320k", parameters=["-ar", "44100", "-sample_fmt", "s16"])

    # Return WAV data as response content
    wav_data = wav_buffer.getvalue()

    return Response(content=wav_data, media_type="audio/wav")

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=5000)
