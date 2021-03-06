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

#include "SpaceWidgetAbstract.h"

namespace Qi
{

SpaceWidgetAbstract::SpaceWidgetAbstract(QWidget *parent)
    : QWidget(parent),
      SpaceWidgetCore(this)
{
}

SpaceWidgetAbstract::~SpaceWidgetAbstract()
{
}

bool SpaceWidgetAbstract::event(QEvent* event)
{
    bool result = QWidget::event(event);
    result |= processOwnerEvent(event);
    return result;
}

void SpaceWidgetAbstract::ensureVisibleImpl(const ID& /*visibleItem*/, const CacheSpace* /*cacheSpace*/, bool /*validateItem*/)
{
    // not applicable
}

} // end namespace Qi
