// Copyright (C) 2015 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "RendererOGL.h"
#include "TextureOGL.h"
#include "RenderTargetOGL.h"
#include "ShaderOGL.h"
#include "Engine.h"
#include "Scene.h"
#include "Camera.h"
#include "Utils.h"
#include "View.h"

namespace ouzel
{
    RendererOGL::RendererOGL(Engine* engine):
        Renderer(engine, Driver::OPENGL)
    {
        
    }
    
    bool RendererOGL::initOpenGL(uint32_t width, uint32_t height)
    {
        //glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        
        glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        //glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
        
        // precomputed alpha
        glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
        
        if (checkOpenGLErrors())
        {
            return false;
        }
        
        Shader* textureShader = _engine->getRenderer()->loadShader("texture.fsh", "texture.vsh");
        _shaders[SHADER_TEXTURE] = textureShader;
        
        Shader* colorShader = _engine->getRenderer()->loadShader("color.fsh", "color.vsh");
        _shaders[SHADER_COLOR] = colorShader;
        
        _view = new View(Size2(width, height), this);
        
        return true;
    }

    void RendererOGL::recalculateProjection()
    {
        Renderer::recalculateProjection();
        
        if (_view)
        {
            glViewport(0, 0, _view->getSize().width, _view->getSize().height);
        }
    }
    
    void RendererOGL::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        checkOpenGLErrors();
    }
    
    void RendererOGL::flush()
    {
        glFlush();
        checkOpenGLErrors();
    }
    
    Texture* RendererOGL::loadTexture(const std::string& filename)
    {
        TextureOGL* texture = new TextureOGL(filename, this);
        
        return texture;
    }
    
    void RendererOGL::activateTexture(Texture* texture, uint32_t layer)
    {
        TextureOGL* textureOGL = static_cast<TextureOGL*>(texture);
        
        glActiveTexture(GL_TEXTURE0 + layer);
        glBindTexture(GL_TEXTURE_2D, textureOGL->getTextureId());
    }
    
    Shader* RendererOGL::loadShader(const std::string& fragmentShader, const std::string& vertexShader)
    {
        ShaderOGL* shader = new ShaderOGL(fragmentShader, vertexShader, this);
        
        return shader;
    }
    
    void RendererOGL::activateShader(Shader* shader)
    {
        ShaderOGL* shaderOGL = static_cast<ShaderOGL*>(shader);
        
        glUseProgram(shaderOGL->getProgramId());
    }
    
    void RendererOGL::drawLine(const Vector2& start, const Vector2& finish, const Vector3& color, const Matrix4& transform)
    {
        GLuint vertexArray = 0;
        GLuint vertexBuffer = 0;
        GLuint colorBuffer = 0;
        GLuint indexBuffer = 0;
        
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);
        
        GLfloat vertices[] = {
            start.x, start.y, -10.0f, 1.0f,
            finish.x, finish.y, -10.0f, 1.0f};
        
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);
        
        GLfloat colors[] = {
            color.x, color.y, color.z, 1.0f,
            color.x, color.y, color.z, 1.0f
        };
        
        glGenBuffers(1, &colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);
        
        GLubyte indices[] = {0, 1};
        
        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        ShaderOGL* colorShader = static_cast<ShaderOGL*>(getShader(SHADER_COLOR));
        
        glUseProgram(colorShader->getProgramId());
        
        GLint uniProj = glGetUniformLocation(colorShader->getProgramId(), "proj");
        glUniformMatrix4fv(uniProj, 1, GL_FALSE, _projection.m);
        
        GLint uniView = glGetUniformLocation(colorShader->getProgramId(), "view");
        
        Camera* camera = _engine->getScene()->getCamera();
        
        if (camera)
        {
            glUniformMatrix4fv(uniView, 1, GL_FALSE, camera->getTransform().m);
        }
        else
        {
            Matrix4 temp;
            glUniformMatrix4fv(uniView, 1, GL_FALSE, temp.m);
        }
        
        GLint uniModel = glGetUniformLocation(colorShader->getProgramId(), "model");
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, transform.m);
        
        glBindVertexArray(vertexArray);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glDrawElements(GL_LINE_STRIP, 2, GL_UNSIGNED_BYTE, nullptr);
        
        // delete buffers
        glDeleteVertexArrays(1, &vertexArray);
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &colorBuffer);
        glDeleteBuffers(1, &indexBuffer);
    }
    
    void RendererOGL::drawRectangle(const Rectangle& rectangle, const Vector3& color, const Matrix4& transform)
    {
        GLuint vertexArray = 0;
        GLuint vertexBuffer = 0;
        GLuint colorBuffer = 0;
        GLuint indexBuffer = 0;
        
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);
        
        GLfloat vertices[] = {
            rectangle.x, rectangle.y, -10.0f, 1.0f,
            rectangle.x + rectangle.width, rectangle.y, -10.0f, 1.0f,
            rectangle.x, rectangle.y + rectangle.height, -10.0f, 1.0f,
            rectangle.x + rectangle.width, rectangle.y + rectangle.height, -10.0f, 1.0f};
        
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);
        
        GLfloat colors[] = {
            color.x, color.y, color.z, 1.0f,
            color.x, color.y, color.z, 1.0f,
            color.x, color.y, color.z, 1.0f,
            color.x, color.y, color.z, 1.0f,
        };
        
        glGenBuffers(1, &colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);
        
        GLubyte indices[] = {0, 1, 3, 2, 0};
        
        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        ShaderOGL* colorShader = static_cast<ShaderOGL*>(getShader(SHADER_COLOR));
        
        glUseProgram(colorShader->getProgramId());
        
        GLint uniProj = glGetUniformLocation(colorShader->getProgramId(), "proj");
        glUniformMatrix4fv(uniProj, 1, GL_FALSE, _projection.m);
        
        GLint uniView = glGetUniformLocation(colorShader->getProgramId(), "view");
        
        Camera* camera = _engine->getScene()->getCamera();
        
        if (camera)
        {
            glUniformMatrix4fv(uniView, 1, GL_FALSE, camera->getTransform().m);
        }
        else
        {
            Matrix4 temp;
            glUniformMatrix4fv(uniView, 1, GL_FALSE, temp.m);
        }
        
        GLint uniModel = glGetUniformLocation(colorShader->getProgramId(), "model");
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, transform.m);
        
        glBindVertexArray(vertexArray);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glDrawElements(GL_LINE_STRIP, 5, GL_UNSIGNED_BYTE, nullptr);
        
        // delete buffers
        glDeleteVertexArrays(1, &vertexArray);
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &colorBuffer);
        glDeleteBuffers(1, &indexBuffer);
    }
    
    void RendererOGL::drawQuad(const Rectangle& rectangle, const Vector3& color, const Matrix4& transform)
    {
        GLuint vertexArray = 0;
        GLuint vertexBuffer = 0;
        GLuint colorBuffer = 0;
        GLuint texcoordBuffer = 0;
        GLuint indexBuffer = 0;
        
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);
        
        GLfloat vertices[] = {
            rectangle.x, rectangle.y, -10.0f, 1.0f,
            rectangle.x + rectangle.width, rectangle.y, -10.0f, 1.0f,
            rectangle.x, rectangle.y + rectangle.height, -10.0f, 1.0f,
            rectangle.x + rectangle.width, rectangle.y + rectangle.height, -10.0f, 1.0f};
        
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);
        
        GLfloat colors[] = {
            color.x, color.y, color.z, 1.0f,
            color.x, color.y, color.z, 1.0f,
            color.x, color.y, color.z, 1.0f,
            color.x, color.y, color.z, 1.0f,
        };
        
        glGenBuffers(1, &colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);
        
        GLfloat texcoords[] = {
            0.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f};
        
        glGenBuffers(1, &texcoordBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(2);
        
        GLubyte indices[] = {0, 1, 2, 1, 3, 2};
        
        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        ShaderOGL* colorShader = static_cast<ShaderOGL*>(getShader(SHADER_TEXTURE));
        
        glUseProgram(colorShader->getProgramId());
        
        GLint uniProj = glGetUniformLocation(colorShader->getProgramId(), "proj");
        glUniformMatrix4fv(uniProj, 1, GL_FALSE, _projection.m);
        
        GLint uniView = glGetUniformLocation(colorShader->getProgramId(), "view");
        
        Camera* camera = _engine->getScene()->getCamera();
        
        if (camera)
        {
            glUniformMatrix4fv(uniView, 1, GL_FALSE, camera->getTransform().m);
        }
        else
        {
            Matrix4 temp;
            glUniformMatrix4fv(uniView, 1, GL_FALSE, temp.m);
        }
        
        GLint uniModel = glGetUniformLocation(colorShader->getProgramId(), "model");
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, transform.m);
        
        glBindVertexArray(vertexArray);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
        
        // delete buffers
        glDeleteVertexArrays(1, &vertexArray);
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &colorBuffer);
        glDeleteBuffers(1, &texcoordBuffer);
        glDeleteBuffers(1, &indexBuffer);
    }
}
