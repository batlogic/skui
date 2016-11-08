/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Ruben Van Boxem
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 **/

#include "library.h++"

#include "utility.h++"

#ifndef WINDOWS_LEAN_AND_MEAN
#define WINDOWS_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace skui
{
  namespace core
  {
    namespace implementation
    {
      constexpr char dll_prefix[] = "lib";
      constexpr char dll_suffix[] = ".dll";
      void* load(const string& filename)
      {
        std::array<string, 3> filenames{{filename,
                                         filename + dll_suffix,
                                         dll_prefix + filename + dll_suffix}};
        for(const auto& filename : filenames)
        {
          void* handle = LoadLibraryW(convert_to_utf16(filename).c_str());
          if(handle)
            return handle;
        }
        return nullptr;
      }

      bool unload(void* handle)
      {
        return FreeLibrary(static_cast<HMODULE>(handle));
      }
    }

    library::function_ptr library::resolve_symbol(const string& symbol_name)
    {
      FARPROC function_address = GetProcAddress(static_cast<HMODULE>(native_handle), symbol_name.c_str());
      return function_ptr(function_address);
    }
  }
}