#pragma once

#include <vector>
#include <string_view>
#include <spawn.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

namespace utils {
    class SoundHandler {
        std::vector<pid_t> pids;
        
    public:
        ~SoundHandler()
        {
            this->stopAllSounds();
        }
        
        void startSound(std::string_view filename, std::size_t soundId, bool repeatForever = false)
        {
            const char *argv[] = {
                "/usr/bin/env",
                "play",
                filename.data(),
                nullptr,
                nullptr,
                nullptr,
            };
            if (repeatForever) {
                argv[3] = "repeat";
                argv[4] = "-";
            }

            posix_spawn_file_actions_t file_actions;
            posix_spawn_file_actions_init(&file_actions);
            posix_spawn_file_actions_addclose(&file_actions, STDOUT_FILENO);
            posix_spawn_file_actions_addclose(&file_actions, STDIN_FILENO);
            posix_spawn_file_actions_addclose(&file_actions, STDERR_FILENO);

            pid_t processPid;
            if (posix_spawn(&processPid, "/usr/bin/env", &file_actions, nullptr, const_cast<char **>(argv), environ) == 0) {
                if (this->pids.size() < (soundId + 1))
                    this->pids.resize(soundId + 1);
                this->stopSound(soundId);
                this->pids[soundId] = processPid;
            }
            
            posix_spawn_file_actions_destroy(&file_actions);
        }

        void stopSound(std::size_t soundId)
        {
            if (this->pids[soundId] != 0)
                kill(this->pids[soundId], SIGINT);
            this->pids[soundId] = 0;
            while (true) {
                auto pid = waitpid(-1, NULL, WNOHANG);
                if (pid == 0 || pid == -1)
                    break;
            }
        }

        void stopAllSounds()
        {
            for (std::size_t i = 0; i < this->pids.size(); ++i)
                this->stopSound(i);
        }
    };
};
