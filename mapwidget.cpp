#include "mapwidget.h"
#include <QSlider>
#include <QHBoxLayout>
#include <QTextStream>

MapWidget::MapWidget()
{
    //读取地图信息
    readMap();
    zoom = 50;
    int width = map.width();
    int height = map.height();

    /*1:限定场景的显示区为地图的大小*/
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setSceneRect(-width/2,-height/2,width,height);
    setScene(scene);
    setCacheMode(CacheBackground);/*CacheBackground标志允许缓存视图的背景*/

    /*2:用于地图缩放的滑动条*/
    QSlider *slider = new QSlider;
    slider->setOrientation(Qt::Vertical);
    slider->setRange(1,100);
    slider->setTickInterval(10);/*间隔*/
    slider->setValue(50);
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(slotZoom(int)));
    /*2.1:用于设置放大缩小的图片*/
    QLabel *zoominLabel = new QLabel;
    zoominLabel->setScaledContents(true);
    zoominLabel->setPixmap(QPixmap("zoomin.png"));
    QLabel *zoomoutLabel = new QLabel;
    zoomoutLabel->setScaledContents(true);
    zoomoutLabel->setPixmap(QPixmap("zoomout.png"));
    /*设置缩放控制子布局*/
    QVBoxLayout *zoomLayout = new QVBoxLayout;
    zoomLayout->addWidget(zoominLabel);
    zoomLayout->addWidget(slider);
    zoomLayout->addWidget(zoomoutLabel);

    /*3:坐标显示区域*/
    QLabel *label1 = new QLabel("GraphicsView:");
    viewCoord = new QLabel;
    QLabel *label2 = new QLabel("GraphicsScene:");
    sceneCoord = new QLabel;
    QLabel *label3 = new QLabel("map:");
    mapCoord = new QLabel;
    /*坐标布局*/
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(label1,0,0);
    gridLayout->addWidget(viewCoord,0,1);
    gridLayout->addWidget(label2,1,0);
    gridLayout->addWidget(sceneCoord,1,1);
    gridLayout->addWidget(label3,2,0);
    gridLayout->addWidget(mapCoord,2,1);
    gridLayout->setSizeConstraint(QLayout::SetFixedSize);
    QFrame *coordFrame = new QFrame;
    coordFrame->setLayout(gridLayout);
    /*坐标显示区域布局*/
    QVBoxLayout *coordLayout = new QVBoxLayout;
    coordLayout->addWidget(coordFrame);
    coordLayout->addStretch();
    /*主布局*/
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(zoomLayout);
    mainLayout->addLayout(coordLayout);
    mainLayout->addStretch();
    mainLayout->setMargin(30);
    mainLayout->setSpacing(10);
    setLayout(mainLayout);
    setWindowTitle("Map Widget");
    setMinimumSize(600,400);


}

void MapWidget::readMap()
{
    QString mapName;
    QFile mapFile("maps.txt");
    int ok = mapFile.open(QIODevice::ReadOnly);
    if(ok)
    {
        QTextStream ts(&mapFile);
        if(!ts.atEnd())
        {
            ts>>mapName;
            ts>>x1>>y1>>x2>>y2;
        }
    }
    map.load(mapName);
}

QPointF MapWidget::mapToMap(QPointF p)
{
    QPointF latLon;
    qreal w =sceneRect().width();
    qreal h =sceneRect().height();
    qreal lon = y1-((h/2+p.y())*abs(y1-y2)/h);
    qreal lat = x1+((w/2+p.x())*abs(x1-x2)/w);
    latLon.setX(lat);
    latLon.setY(lon);
    return latLon;
}

void MapWidget::slotZoom(int value)
{
    qreal s;
    if(value>zoom)                              //放大
    {
        s=pow(1.01,(value-zoom));
    }
    else                                        //缩小
    {
        s=pow(1/1.01,(zoom-value));
    }
    scale(s,s);
    zoom = value;
}

void MapWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->drawPixmap(int(sceneRect().x()),int(sceneRect().y()), map);
}

void MapWidget::mouseMoveEvent(QMouseEvent *event)
{
    //坐标
    QPoint viewPoint = event->pos();
    viewCoord->setText(QString::number(viewPoint.x())+","+QString::number(viewPoint.y()));
    //QGraphiceScene 坐标
    QPointF scenePoint = mapToScene(viewPoint);
    sceneCoord->setText(QString::number(scenePoint.x())+","+QString::number(scenePoint.y()));

    //地图坐标
    QPointF latLon = mapToMap(viewPoint);
    mapCoord->setText(QString::number(latLon.x())+","+QString::number(latLon.y()));
}
