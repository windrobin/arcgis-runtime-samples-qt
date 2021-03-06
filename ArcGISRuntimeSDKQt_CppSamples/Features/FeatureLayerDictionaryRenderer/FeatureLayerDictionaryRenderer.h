// [WriteFile Name=FeatureLayerDictionaryRenderer, Category=Features]
// [Legal]
// Copyright 2016 Esri.

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// [Legal]

#ifndef FEATURELAYERDICTIONARYRENDERER_H
#define FEATURELAYERDICTIONARYRENDERER_H

#include <QQuickItem>

namespace Esri
{
    namespace ArcGISRuntime
    {
        class Geodatabase;
        class MapQuickView;
    }
}

class FeatureLayerDictionaryRenderer : public QQuickItem
{
    Q_OBJECT

public:
    explicit FeatureLayerDictionaryRenderer(QQuickItem* parent = 0);
    ~FeatureLayerDictionaryRenderer();

    void componentComplete() Q_DECL_OVERRIDE;

signals:
    void allLayersLoaded();

private:
    QString m_dataPath;
    double m_scaleFactor;
    int m_loadedLayerCount;
    Esri::ArcGISRuntime::MapQuickView* m_mapView;
    Esri::ArcGISRuntime::Geodatabase* m_geodatabase;
};

#endif // FEATURELAYERDICTIONARYRENDERER_H
