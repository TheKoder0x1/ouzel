// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <vector>
#define NOMINMAX
#include <d3d11.h>
#include "graphics/ShaderResource.h"

namespace ouzel
{
    namespace graphics
    {
        class ShaderD3D11: public ShaderResource
        {
        public:
            ShaderD3D11();
            virtual ~ShaderD3D11();

            struct Location
            {
                uint32_t offset;
                uint32_t size;
            };

            const std::vector<Location>& getPixelShaderConstantLocations() const { return pixelShaderConstantLocations; }
            const std::vector<Location>& getVertexShaderConstantLocations() const { return vertexShaderConstantLocations; }

            virtual ID3D11PixelShader* getPixelShader() const { return pixelShader; }
            virtual ID3D11VertexShader* getVertexShader() const { return vertexShader; }

            virtual ID3D11Buffer* getPixelShaderConstantBuffer() const { return pixelShaderConstantBuffer; }
            virtual ID3D11Buffer* getVertexShaderConstantBuffer() const { return vertexShaderConstantBuffer; }
            virtual ID3D11InputLayout* getInputLayout() const { return inputLayout;  }

            bool uploadBuffer(ID3D11Buffer* buffer, const void* data, uint32_t size);

        protected:
            virtual bool upload() override;

            ID3D11PixelShader* pixelShader = nullptr;
            ID3D11VertexShader* vertexShader = nullptr;
            ID3D11InputLayout* inputLayout = nullptr;

            ID3D11Buffer* pixelShaderConstantBuffer = nullptr;
            ID3D11Buffer* vertexShaderConstantBuffer = nullptr;

            std::vector<Location> pixelShaderConstantLocations;
            uint32_t pixelShaderConstantSize = 0;
            std::vector<Location> vertexShaderConstantLocations;
            uint32_t vertexShaderConstantSize = 0;
        };
    } // namespace graphics
} // namespace ouzel
