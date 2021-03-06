/*
   Copyright (c) 2008-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "Link.h"
#include <QDesktopServices>

namespace Qi
{

ViewLink::ViewLink(SharedPtr<ModelText> model, ViewDefaultController createDefaultController, Qt::Alignment alignment, Qt::TextElideMode textElideMode)
    : ViewText(std::move(model), ViewDefaultControllerNone, alignment, textElideMode),
      m_pushableTracker(this)
{
    if (createDefaultController)
    {
        auto controller = makeShared<ControllerMouseLink>();
        controller->onApply = [this] (ID id, const ControllerContext& context) {
            if (action)
                action(id, context, this);
        };
        setController(controller);
    }
}

ViewLink::ViewLink(SharedPtr<ModelText> model, SharedPtr<ModelUrl> modelUrl, Qt::Alignment alignment, Qt::TextElideMode textElideMode)
    : ViewText(model, ViewDefaultControllerNone, alignment, textElideMode),
      m_pushableTracker(this)
{
    auto controller = makeShared<ControllerMouseLink>();
    controller->onApply = [this] (ID id, const ControllerContext& context) {
        if (action)
            action(id, context, this);
    };
    setController(controller);

    action = [modelUrl = std::move(modelUrl)](ID id, const ControllerContext& /*context*/, const ViewLink* /*viewLink*/) {
        QDesktopServices::openUrl(modelUrl->value(id));
    };
}

void ViewLink::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    PainterState pState;
    pState.save(painter);

    QColor linkColor = ctx.palette().color(ctx.colorGroup(), QPalette::Link);
    MousePushState state = m_pushableTracker.pushStateByItem(cache.id);
    switch (state)
    {
    case MousePushStateHot:
        linkColor = linkColor.lighter();
        break;

    case MousePushStatePushed:
        linkColor = linkColor.darker();
        break;

    default:;
    }

    painter->setPen(linkColor);

    QRect rect = cache.cacheView.rect();
    QString text = theModel()->value(cache.id);
    Qt::TextElideMode elideMode = textElideMode(cache.id);
    if (elideMode != Qt::ElideNone)
    {
        QString elidedText = painter->fontMetrics().elidedText(text, elideMode, rect.width());
        if (showTooltip)
            *showTooltip = (elidedText != text);

        text = elidedText;
    }
    else
    {
        if (showTooltip)
            *showTooltip = (painter->fontMetrics().width(text) > rect.width());
    }

    painter->drawText(rect, alignment(cache.id), text);

    pState.restore(painter);
}

ControllerMouseLink::ControllerMouseLink(ControllerMousePriority priority, bool processDblClick)
    : ControllerMousePushableCallback(priority, processDblClick)
{
}

void ControllerMouseLink::activateImpl(const ActivationInfo& activationInfo)
{
    ControllerMousePushableCallback::activateImpl(activationInfo);

    activationInfo.context.pushCursor(Qt::PointingHandCursor, this);
}

void ControllerMouseLink::deactivateImpl()
{
    activationState().context.popCursor(this);

    ControllerMousePushableCallback::deactivateImpl();
}


} // end namespace Qi
