// Copyright 2016 The SwiftShader Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef D3D9_Direct3DIndexBuffer9_hpp
#define D3D9_Direct3DIndexBuffer9_hpp

#include "Direct3DResource9.hpp"

#include <d3d9.h>

namespace sw
{
	class Resource;
}

namespace D3D9
{
	class Direct3DIndexBuffer9 : public IDirect3DIndexBuffer9, public Direct3DResource9
	{
	public:
		Direct3DIndexBuffer9(Direct3DDevice9 *device, unsigned int length, unsigned long usage, D3DFORMAT format, D3DPOOL pool);

		~Direct3DIndexBuffer9() override;

		// IUnknown methods
		long __stdcall QueryInterface(const IID &iid, void **object) override;
		unsigned long __stdcall AddRef() override;
		unsigned long __stdcall Release() override;

		// IDirect3DResource9 methods
		long __stdcall FreePrivateData(const GUID &guid) override;
		long __stdcall GetPrivateData(const GUID &guid, void *data, unsigned long *size) override;
		void __stdcall PreLoad() override;
		long __stdcall SetPrivateData(const GUID &guid, const void *data, unsigned long size, unsigned long flags) override;
		long __stdcall GetDevice(IDirect3DDevice9 **device) override;
		unsigned long __stdcall SetPriority(unsigned long newPriority) override;
		unsigned long __stdcall GetPriority() override;
		D3DRESOURCETYPE __stdcall GetType() override;

		// IDirect3DIndexBuffer9 methods
		long __stdcall GetDesc(D3DINDEXBUFFER_DESC *description) override;
		long __stdcall Lock(unsigned int offset, unsigned int size, void **data, unsigned long flags) override;
		long __stdcall Unlock() override;

		// Internal methods
		sw::Resource *getResource() const;
		bool is32Bit() const;

	private:
		// Creation parameters
		const unsigned int length;
		const long usage;
		const D3DFORMAT format;

		sw::Resource *indexBuffer;
		int lockCount;
	};
}

#endif // D3D9_Direct3DIndexBuffer9_hpp
