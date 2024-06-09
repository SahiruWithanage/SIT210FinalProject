import uvicorn
from fastapi import FastAPI, Response
from pydub import AudioSegment
import io
import subprocess

app = FastAPI()

@app.get("/stream")
async def stream_audio():
    def generate():
        cmd = [
            'ffmpeg', '-i', '/tmp/librespot_pipe', '-f', 's16le', '-acodec', 'pcm_s16le', '-ar', '44100', '-ac', '1', 'pipe:1'
        ]
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        while True:
            chunk = process.stdout.read(4096)
            if not chunk:
                break
            yield chunk

    return Response(generate(), media_type="audio/wav")

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)
