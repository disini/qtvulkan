/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the QtVulkan module
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QGuiApplication>
#include <QWindow>
#include "worker.h"

int main(int argc, char **argv)
{
    // Let's report what is going on under the hood.
    qputenv("QVULKAN_DEBUG", "render");

    QGuiApplication app(argc, argv);

    qDebug("Opening window. Main/gui thread is %p", QThread::currentThread());

    QWindow window;
    window.setSurfaceType(QSurface::OpenGLSurface);

    // Attach a Vulkan renderer to our window.
    QVulkanRenderLoop rl(&window);

    // Default is FIFO mode (vsync, throttle the thread), validation off, no continuous update requests, 1 frame in flight.
    // Change this a bit:
    rl.setFlags(QVulkanRenderLoop::UpdateContinuously | QVulkanRenderLoop::EnableValidation /* | QVulkanRenderLoop::Unthrottled */);
    rl.setFramesInFlight(FRAMES_IN_FLIGHT);

    // Attach our worker to the Vulkan renderer. Note that while the worker
    // object lives on the main/gui thread, its functions will get invoked on
    // the renderer's dedicated thread.
    Worker worker(&rl);
    rl.setWorker(&worker);

    window.resize(1024, 768);

    // Go! Once exposed, rendering will start.
    window.show();

    return app.exec();
}
