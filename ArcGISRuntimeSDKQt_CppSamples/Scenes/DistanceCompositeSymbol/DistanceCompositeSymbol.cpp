// [WriteFile Name=DistanceCompositeSymbol, Category=Scenes]
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

#include "DistanceCompositeSymbol.h"

#include "Scene.h"
#include "SceneQuickView.h"
#include "Basemap.h"
#include "ArcGISTiledElevationSource.h"
#include "DistanceCompositeSceneSymbol.h"
#include "SimpleMarkerSymbol.h"
#include "SimpleMarkerSceneSymbol.h"
#include "ModelSceneSymbol.h"
#include <QQmlProperty>

using namespace Esri::ArcGISRuntime;

DistanceCompositeSymbol::DistanceCompositeSymbol(QQuickItem* parent) :
    QQuickItem(parent),
    m_scene(nullptr),
    m_sceneView(nullptr)
{
}

DistanceCompositeSymbol::~DistanceCompositeSymbol()
{
}

void DistanceCompositeSymbol::componentComplete()
{
    QQuickItem::componentComplete();

    // get the data path
    QString dataPath = QQmlProperty::read(this, "dataPath").toString();

    // find QML SceneView component
    m_sceneView = findChild<SceneQuickView*>("sceneView");

    // create a new basemap instance
    Basemap* basemap = Basemap::imagery(this);
    // create a new scene instance
    m_scene = new Scene(basemap, this);
    // set scene on the scene view
    m_sceneView->setArcGISScene(m_scene);

    // create a new elevation source
    ArcGISTiledElevationSource* elevationSource = new ArcGISTiledElevationSource(QUrl("http://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer"), this);
    // add the elevation source to the scene to display elevation
    m_scene->baseSurface()->elevationSources()->append(elevationSource);

    // create a camera
    Point point(-2.708471, 56.096575, 5000, m_sceneView->spatialReference());
    Camera camera(point, 1500, 0, 80.0, 0);

    // create a new graphics overlay and add it to the sceneview
    GraphicsOverlay* graphicsOverlay = new GraphicsOverlay(this);
    graphicsOverlay->setSceneProperties(LayerSceneProperties(SurfacePlacement::Relative));
    m_sceneView->graphicsOverlays()->append(graphicsOverlay);

    //! [create model scene symbol]
    ModelSceneSymbol* mms = new ModelSceneSymbol(QUrl(dataPath), 0.01f, this);
    mms->setHeading(180);
    //! [create model scene symbol]

    connect(mms, &ModelSceneSymbol::loadStatusChanged, this, [mms, point, graphicsOverlay, this](){
        if (mms->loadStatus() == LoadStatus::Loaded)
        {
            SimpleMarkerSymbol* sms = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor("red"), 10.0f, this);
            //! [create simple marker scene symbol]
            SimpleMarkerSceneSymbol* smss = new SimpleMarkerSceneSymbol(SimpleMarkerSceneSymbolStyle::Cone, QColor("red"), 75, 75, 75, SceneSymbolAnchorPosition::Bottom, this);
            //! [create simple marker scene symbol]

            //! [create distance symbol ranges with each symbol type and a distance range(meters)]
            DistanceSymbolRange* dsrModel = new DistanceSymbolRange(mms, 0, 999, this); // ModelSceneSymbol
            DistanceSymbolRange* dsrCone = new DistanceSymbolRange(smss, 1000, 1999, this); // SimpleMarkerSceneSymbol
            DistanceSymbolRange* dsrCircle = new DistanceSymbolRange(sms, 2000, 0, this); // SimpleMarkerSymbol

            DistanceCompositeSceneSymbol* compositeSceneSymbol = new DistanceCompositeSceneSymbol(this);

            compositeSceneSymbol->ranges()->append(dsrModel);
            compositeSceneSymbol->ranges()->append(dsrCone);
            compositeSceneSymbol->ranges()->append(dsrCircle);
            //! [create distance symbol ranges with each symbol type and a distance range(meters)]

            // create a graphic using the composite symbol
            Graphic* graphic = new Graphic(point, compositeSceneSymbol, this);
            // add the graphic to the graphics overlay
            graphicsOverlay->graphics()->append(graphic);
        }
    });

    mms->load();

    // set the viewpoint
    m_sceneView->setViewpointCameraAndWait(camera);
}

