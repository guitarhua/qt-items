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

#ifndef QI_CACHE_SPACE_GRID_H
#define QI_CACHE_SPACE_GRID_H

#include "space/CacheSpace.h"
#include "SpaceGrid.h"

namespace Qi
{

class QI_EXPORT CacheSpaceGrid: public CacheSpace
{
    Q_OBJECT
    Q_DISABLE_COPY(CacheSpaceGrid)

public:
    explicit CacheSpaceGrid(SharedPtr<SpaceGrid> grid);
    ~CacheSpaceGrid();

    const SharedPtr<SpaceGrid>& spaceGrid() const { return m_grid; }
    bool isEmpty() const { return m_items.isEmpty(); }

    bool isItemInFrame(GridID visibleId) const;
    bool isItemInFrameStrict(GridID visibleId) const;
    bool isItemAbsInFrame(GridID absId) const;

    void visibleItemsRange(GridID& idStart, GridID& idEnd) const;
    GridID visibleItemByPosition(QPoint point) const;

private:
    void clearItemsCacheImpl() const override;
    void validateItemsCacheImpl() const override;
    bool forEachCacheItemImpl(const std::function<bool(const SharedPtr<CacheItem>&)>& visitor) const override;
    const CacheItem* cacheItemImpl(ID visibleId) const override;
    const CacheItem* cacheItemByPositionImpl(QPoint point) const override;

    // source grid space
    SharedPtr<SpaceGrid> m_grid;

    // visible item ids
    mutable GridID m_idStart;
    mutable GridID m_idEnd;
    // caches items
    mutable QVector<SharedPtr<CacheItem>> m_items;
};

} // end namespace Qi 

#endif // QI_CACHE_SPACE_GRID_H
