#include "game/audio/Bank.h"

#include <fstream>
#include <sstream>

#include "SDL_log.h"
#include "SDL_mixer.h"
#include "document.h"
#include "game/audio/AudioSystem.h"
#include "game/audio/Event.h"
#include "game/audio/Helper.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/schema.h"
#include "rapidjson/stringbuffer.h"

Bank::Bank() {}
Bank::~Bank() {}

/*
Bankデータをロードする
ロードに成功したらtrue，失敗したらfalseを返す
Bankデータはjson形式のファイルで記述する
*/
bool Bank::Load(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        SDL_Log("File not found: Bank %s", fileName.c_str());
        return false;
    }
    rapidjson::IStreamWrapper isw(file);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    if (!doc.IsObject()) {
        SDL_Log("Invalid bank data: %s", fileName.c_str());
        return false;
    }

    // versionごとにフォーマットを分ける
    int ver = -1;
    if (doc.HasMember("version")) {
        ver = doc["version"].GetInt();
    }
    switch (ver) {
        case 1:
            return LoadVersion1(doc);

        default:
            SDL_Log("Invalid bank file version: %s", fileName.c_str());
            return false;
    }
}

/*
Notes: ver1のフォーマット
{
    "bankName": "MainBank",
    "version": 1,
    "events": [
        {
            "id": "bgm_main",
            "file": "audio/bgm_main.ogg",
            "is3D": false,
            "stream": false,
            "loop": true,
            "volume": 1.0,
            "pitch": 1.0
        },
        {
            :
        },
    ]
}
*/
bool Bank::LoadVersion1(rapidjson::Document& doc) {
    // スキーマ読み込み
    const char* schemaJson = R"({
        "type": "object",
        "required": ["bankName", "version", "events"],
        "properties": {
            "bankName": {"type": "string"},
            "version": {"type": "integer"},
            "events": {
                "type": "array",
                "items": {
                    "type": "object",
                    "required": ["id", "file"],
                    "properties": {
                        "id": {"type": "string"},
                        "file": {"type": "string"},
                        "is3D": {"type": "boolean"},
                        "stream": {"type": "boolean"},
                        "loop": {"type": "boolean"},
                        "volume": {"type": "number"},
                        "pitch": {"type": "number"}
                    }
                }
            }
        }
    })";
    rapidjson::Document schemaDoc;
    schemaDoc.Parse(schemaJson);

    // バリデーション
    rapidjson::SchemaDocument schema(schemaDoc);
    rapidjson::SchemaValidator validator(schema);

    if (!doc.Accept(validator)) {
        SDL_Log("Invalid schema");
        return false;
    }

    // イベント情報のパース
    const rapidjson::Value& events = doc["events"];

    // サウンドデータを保持するためのバッファ作成
    std::vector<ALuint> buffers(events.Size());
    alGetError();
    alGenBuffers(events.Size(), buffers.data());
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        SDL_Log("Failed to create OpenAL buffer: %s", alGetErrorString(error));
        return false;
    }

    for (rapidjson::SizeType i = 0; i < events.Size(); i++) {
        // サウンドファイルのロード，格納
        std::string soundFileName = events[i]["file"].GetString();
        auto soundIter = mSounds.find(soundFileName);
        if (soundIter == mSounds.end()) {
            // サウンドファイルのロード，PCMデータに変換
            Mix_Chunk* chunk = Mix_LoadWAV(soundFileName.c_str());
            if (!chunk) {
                SDL_Log("Failed to load sound file: %s", Mix_GetError());
                return false;
            }
            int pcmFrequency, pcmChannels;
            Uint16 pcmFormat;
            if (!Mix_QuerySpec(&pcmFrequency, &pcmFormat, &pcmChannels)) {
                SDL_Log("Failed to get pcm spec");
            }

            // 周波数，チャンネル，フォーマットからPCMデータを整形
            ALsizei frequency = pcmFrequency;
            ALenum format = AL_FORMAT_MONO16;
            if (pcmChannels == 2) {
                frequency *= 2;
            }

            // PCMデータをバッファに流し込む
            alGetError();
            alBufferData(buffers[i], format, chunk->abuf,
                         static_cast<ALsizei>(chunk->alen), frequency);
            Mix_FreeChunk(chunk);
            ALenum error = alGetError();
            if (error != AL_NO_ERROR) {
                SDL_Log("Failed to fill buffer audio data: %s",
                        alGetErrorString(error));
                return false;
            }
            mSounds.emplace(soundFileName, buffers[i]);
        }

        // イベントデータの作成，格納
        std::string id = events[i]["id"].GetString();
        auto eventIter = mEvents.find(id);
        if (eventIter == mEvents.end()) {
            Event* e = new Event(this);
            e->mSoundID = soundFileName;
            e->mIs3D = events[i]["is3D"].GetBool();
            e->mIsStream = events[i]["stream"].GetBool();
            e->mIsLoop = events[i]["loop"].GetBool();
            e->mVolume = events[i]["volume"].GetFloat();
            e->mPitch = events[i]["pitch"].GetFloat();

            mEvents.emplace(id, e);
        }
    }
    return true;
}

bool Bank::Unload() {
    // イベントのメモリ解放
    for (auto event : mEvents) {
        delete event.second;
    }

    alGetError();
    // サウンドバッファのメモリ開放
    std::vector<ALuint> buffers;
    buffers.reserve(mSounds.size());
    for (auto& sound : mSounds) {
        buffers.push_back(sound.second);
    }
    alGetError();
    alDeleteBuffers(buffers.size(), buffers.data());
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        SDL_Log("Failed to delete buffer: %s", alGetErrorString(error));
        return false;
    }

    mEvents.clear();
    mSounds.clear();

    return true;
}
