//
//  diagnostic_message_handler.h
//  sun_ray
//
//  BSD 3-Clause License
//  Copyright (c) 2020, Lars-Christian Fürstenberg
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice,
//  this list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//  3. Neither the name of the copyright holder nor the names of its
//  contributors may be used to endorse or promote products derived from this
//  software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//

#pragma once

#include <sun_ray/script/location.h>

#include <algorithm>
#include <iterator>
#include <vector>


namespace sunray
{
  namespace script
  {
    enum class DiagnosticMessageLevel { NONE, ERROR_LVL, WARNING_LVL };

    template<class CharT, class Traits>
    std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const DiagnosticMessageLevel& lvl)
    {
      switch (lvl) {
        case DiagnosticMessageLevel::NONE:
          os << "NONE";
          break;
        case DiagnosticMessageLevel::ERROR_LVL:
          os << "ERROR";
          break;
        case DiagnosticMessageLevel::WARNING_LVL:
          os << "WARNING";
          break;
      }
      return os;
    }


    class DiagnosticMessageHandler
    {
    public:
      DiagnosticMessageHandler() = default;

      ~DiagnosticMessageHandler() = default;

      DiagnosticMessageHandler(const DiagnosticMessageHandler&) = delete;
      DiagnosticMessageHandler(DiagnosticMessageHandler&&) = delete;
      DiagnosticMessageHandler& operator=(const DiagnosticMessageHandler&) = delete;
      DiagnosticMessageHandler& operator=(DiagnosticMessageHandler&&) = delete;

      void add_error(const std::string& code, const std::string& message, const Location& location)
      {
        messages_.emplace_back(DiagnosticMessage{DiagnosticMessageLevel::ERROR_LVL, code, message, location});
      }

      void add_warning(const std::string& code, const std::string& message, const Location& location)
      {
        messages_.emplace_back(DiagnosticMessage{DiagnosticMessageLevel::WARNING_LVL, code, message, location});
      }

      bool has_error() const
      {
        return std::any_of(messages_.begin(), messages_.end(), [](const DiagnosticMessage& m) {
          return m.level == DiagnosticMessageLevel::ERROR_LVL;
        });
      }

      bool has_warning() const
      {
        return std::any_of(messages_.begin(), messages_.end(), [](const DiagnosticMessage& m) {
          return m.level == DiagnosticMessageLevel::WARNING_LVL;
        });
      }

      void output_messages(std::ostream& stream, DiagnosticMessageLevel lvl = DiagnosticMessageLevel::NONE) const
      {
        std::copy_if(messages_.begin(), messages_.end(), std::ostream_iterator<DiagnosticMessage>(stream, "\n"),
                     [&lvl](const DiagnosticMessage& message) {
                       return lvl == DiagnosticMessageLevel::NONE || lvl == message.level;
                     });
      }

      void reset()
      {
        messages_.clear();
      }

    private:
      struct DiagnosticMessage {
        DiagnosticMessageLevel level;
        std::string code;
        std::string message;
        Location location;
      };

      friend std::ostream& operator<<(std::ostream& stream, const DiagnosticMessage& message)
      {
        stream << message.level << " [" << message.code << "]: " << message.message << " " << message.location;
        return stream;
      }

      std::vector<DiagnosticMessage> messages_;
    };
  }
}
