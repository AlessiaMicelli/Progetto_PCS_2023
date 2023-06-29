#ifndef __TEST_EMPTY_H
#define __TEST_EMPTY_H

#include <gtest/gtest.h>
#include "empty_class.hpp"


using namespace testing;
using namespace dalaunay;

TEST(TestDelaunay, TestOrdinaPunti)
{

    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,0.0);

    Triangle Triangolo(p1,p2,p3);
    Triangle TriangoloOrdinato(p1,p3,p2);

    EXPECT_EQ(Triangolo, TriangoloOrdinato);
}

TEST(TestDelaunay,TestIsPointOutside)
{
    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,0.0);
    Point pext(5.0,6.0);
    Triangle TriangoloOrdinato(p1,p3,p2);

    EXPECT_EQ(ispointin(TriangoloOrdinato,pext), false);
}

TEST(TestDelaunay,TestIsPointOutside2)
{
    Point p1(0.915736,0.138624);
    Point p2(0.0971318,0.917194) ;
    Point p3(0.0357117,0.254282);
    Point pext(0.814724,0.276025) ;
    Triangle TriangoloOrdinato(p1,p3,p2);

    EXPECT_EQ(ispointin(TriangoloOrdinato,pext), false);
}

TEST(TestDelaunay,TestIsPointInside)
{
    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,0.0);
    Point pint(0.25,0.25);
    Triangle TriangoloOrdinato(p1,p3,p2);

    EXPECT_EQ(ispointin(TriangoloOrdinato,pint), true);
}

TEST(TestDelaunay,TestIsPointBetween)
{
    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,0.0);
    Point pbetween(0.6,0.6);
    Triangle TriangoloOrdinato(p1,p3,p2);

    EXPECT_EQ(ispointin(TriangoloOrdinato,pbetween), false);
}

TEST(TestDelaunay,TestMaxDistance)
{

    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,0.0);
    Point p4(10.0, 10.0);
    Point p5(2.0,0.0);
    Point p6(0.0,2.0);
    Point p7(1.5,0.0);
    Point p8(0.0,1.2);
    Point p9(5.0,20.0);
    Point p10(0.6,0.6);

    int a;
    int b;
    int c;
    int id=0;
    vector<Point> punti;
    punti.push_back(p1);id++;
    punti.push_back(p2);id++;
    punti.push_back(p3);id++;
    punti.push_back(p4);id++;
    punti.push_back(p5);id++;
    punti.push_back(p6);id++;
    punti.push_back(p7);id++;
    punti.push_back(p8);id++;
    punti.push_back(p9);id++;
    punti.push_back(p10);id++;

    Distanzamax( punti, a, b, c);
    EXPECT_EQ(punti[a]+punti[b]+punti[c],p1+p4+p9);
}

TEST(TestDelaunay,TestIntersectionSegments)
{
    Point p1(0.0,0.0);
    Point p2(2.0,0.0);
    Point p3(1.0,-1.0);
    Point p4(1.0,1.0);


    EXPECT_EQ(siintersecano(p1,p2,p3,p4),true);
}
TEST(TestDelaunay,TestIntersectionSegments2)
{
    Point p1(0.0,0.0);
    Point p2(2.0,0.0);
    Point p3(3.0,-1.0);
    Point p4(3.0,1.0);

    EXPECT_EQ(siintersecano(p1,p2,p3,p4),false);
}

TEST(TestDelaunay,TestIntersectionSegments3)
{
    Point p1(0.0,0.0);
    Point p2(2.0,1.0);
    Point p3(1.0,-2.0);
    Point p4(3.0,2.0);

    EXPECT_EQ(siintersecano(p1,p2,p3,p4),false);
}
/*
TEST(TestDelaunay, TestFlip)
{
    Point p1(0.0,0.0);
    Point p2(1.0,0.0);
    Point p3(0.0,1.0);
    Triangle ABC (&p1,&p2,&p3);
    Point p4(2.0, 1.0);
    Triangle CBD (&p3,&p2,&p4);
    Lati lato(&p1,&p2);
    //Lati &l=lato;
    Flip(ABC, CBD,lato);
    Triangle ADB (&p1,&p4,&p2);
    Triangle ADC (&p1,&p4,&p3);
    EXPECT_EQ(ABC,ADB);
    EXPECT_EQ(CBD,ADC);
    EXPECT_EQ(lato,Lati(&p2,&p3));


}
*/
TEST(TestDelaunay,TestCreoTriangoli){
    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,0.0);
    Point pInt(0.25,0.25);
    int i=1;
    int j=0;

    Triangle Triangolo(p1,p3,p2);

    //vector<Triangle> ListaTriangoli;
    //vector<Triangle> ListaTriangoliOutput;
    vector <Lati> lati;
    vector<Triangle> ListaTriangoli;
    ListaTriangoli.push_back(Triangolo);

    Triangle Triangolo1(p1,p3,pInt);
    Triangle Triangolo2(pInt,p3,p2);
    Triangle Triangolo3(p1,pInt,p2);
    vector<Triangle> ListaTriangoliOutput;
    ListaTriangoliOutput.push_back(Triangolo1);
    ListaTriangoliOutput.push_back(Triangolo2);
    ListaTriangoliOutput.push_back(Triangolo3);
    triangoli_int(Triangolo,pInt,lati,ListaTriangoli,i,j);//,lati

    EXPECT_EQ(ListaTriangoli,ListaTriangoliOutput);
}

TEST(TestDelaunay,TestTrovaTriangolo)
{
    Point p1(0.0,0.0);
    Point p2(1.0,0.0);
    Point p3(0.0,1.0);
    Lati lato(p2,p3);
    Triangle abc (p1,p2,p3);
    abc.id=2;
    Point p4(2.0, 1.0);
    Triangle cbd (p3,p2,p4);
    cbd.id=4;
    vector<Triangle> ListaTriangoliOutput;
    ListaTriangoliOutput.push_back(abc);
    ListaTriangoliOutput.push_back(cbd);

    EXPECT_EQ(trovatriangolo(ListaTriangoliOutput,lato,2), 1);
}

TEST(TestDelaunay, TestInput)
{
    vector <Point> points;
    string InputFilePath = "C:/Users/Alessia Micelli/Desktop/PCS2023_Exercises/Projects/Delaunay/Dataset/Test2.csv";
    vector<Point> punti{
    Point(0.814723686393179, 0.276025076998578),
    Point( 0.905791937075619, 0.679702676853675),
    Point( 0.126986816293506, 0.655098003973841),
    Point( 0.913375856139019, 0.162611735194631),
    Point( 0.63235924622541, 0.118997681558377),
    Point( 0.0975404049994095, 0.498364051982143),
    Point( 0.278498218867048, 0.959743958516081),
    Point( 0.546881519204984, 0.340385726666133),
    Point( 0.957506835434298, 0.585267750979777),
    Point( 0.964888535199277, 0.223811939491137),
    Point( 0.157613081677548, 0.751267059305653),
    Point( 0.970592781760616 ,0.255095115459269),
    Point( 0.957166948242946, 0.505957051665142),
    Point( 0.485375648722841, 0.699076722656686),
    Point( 0.8002804688888, 0.890903252535798),
    Point( 0.141886338627215, 0.959291425205444),
    Point( 0.421761282626275, 0.547215529963803),
    Point( 0.915735525189067, 0.138624442828679),
    Point( 0.792207329559554, 0.149294005559057),
    Point( 0.959492426392903, 0.257508254123736),
    Point( 0.655740699156587, 0.840717255983663),
    Point( 0.0357116785741896, 0.254282178971531),
    Point( 0.849129305868777, 0.814284826068816),
    Point( 0.933993247757551, 0.243524968724989),
    Point( 0.678735154857773, 0.929263623187228),
    Point( 0.757740130578333, 0.349983765984809),
    Point( 0.743132468124916, 0.196595250431208),
    Point( 0.392227019534168, 0.251083857976031),
    Point( 0.655477890177557, 0.616044676146639),
    Point( 0.171186687811562, 0.473288848902729),
    Point( 0.706046088019609, 0.351659507062997),
    Point( 0.0318328463774207, 0.830828627896291),
    Point( 0.27692298496089, 0.585264091152724),
    Point( 0.0461713906311539, 0.54972360829114),
    Point( 0.0971317812358475, 0.91719366382981),
    Point( 0.823457828327293, 0.285839018820374),
    Point( 0.694828622975817, 0.757200229110721),
    Point( 0.317099480060861, 0.753729094278495),
    Point( 0.950222048838355, 0.380445846975357),
    Point( 0.0344460805029088, 0.567821640725221),
    Point( 0.438744359656398, 0.0758542895630636),
    Point( 0.381558457093008, 0.0539501186666071),
    Point( 0.765516788149002, 0.530797553008973),
    Point( 0.795199901137063, 0.779167230102011),
    Point( 0.186872604554379, 0.934010684229183),
    Point( 0.489764395788231, 0.12990620847373),
    Point( 0.445586200710899, 0.568823660872193),
    Point( 0.646313010111265, 0.469390641058206),
    Point( 0.709364830858073, 0.0119020695012414),
    Point(0.754686681982361, 0.337122644398882)};

    //InputFile(InputFilePath,points);
    //EXPECT_EQ(points,punti);
}

TEST(TestDelaunay, TestInput2)
{
    vector<Point> points;
    string InputFilePath = "C:/Users/Alessia Micelli/Desktop/PCS2023_Exercises/Projects/Delaunay/Dataset/prova.csv";
    vector<Point> punti;
    int id=0;
    punti.push_back(Point(0.0000000000000000e+00, 0.0000000000000000e+00));id++;
    punti.push_back(Point( 1.5000000000000000e+00 ,0.0000000000000000e+00));id++;
    punti.push_back(Point( 1.0000000000000000e+00, 1.0000000000000000e+00));id++;
    punti.push_back(Point( 0.1000000000000000e+00 ,1.1000000000000000e+00));id++;
    punti.push_back(Point( 5.0000000000000000e-01, 4.0000000000000000e-01));id++;
    punti.push_back(Point( 1.1000000000000000e+00 ,3.0000000000000000e-01));id++;
    punti.push_back(Point( 2.5000000000000000e-01, 0.7000000000000000e+00));id++;
    punti.push_back(Point(1.3000000000000000e-00 ,1.5000000000000000e+00));id++;
    punti.push_back(Point(6.0000000000000000e-01, 1.10000000000000000e+00));id++;
    punti.push_back(Point(4.0000000000000000e-01 ,0.3000000000000000e+00));id++;
    punti.push_back(Point (0.4,1.3));
    punti.push_back(Point (0.4,1.4));

    InputFile(InputFilePath,points);
    EXPECT_EQ(points,punti);

}

/*
TEST(TestDelaunay,TestOutput)
{
    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,0.0);
    Point p4(1.0,1.0);
    Point p5(2.0,2.0);
    Point p6(3.4,3.1);
    vector<Lati> lativerifica{ Lati(&p1, &p2), Lati(&p3,&p4), Lati(&p5,&p6)};
    //string b = to_string(1) + ") punto di inizio: (" + to_string(p1.x) + to_string(p1.y)+")       punto di fine: (" + to_string(p2.x)+","+to_string(p2.y)+")";
    output (lativerifica);
    //EXPECT_EQ(b, a);


}
*/

TEST(TestDelaunay, TestFileOutput)
{
    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,0.0);
    Point p4(1.0,1.0);
    Point p5(2.0,2.0);
    Point p6(3.4,3.1);
    vector<Lati> myList{Lati(&p1, &p2), Lati(&p3, &p4), Lati(&p5, &p6)};
    string outputFilePath = "C:/Users/Alessia Micelli/Desktop/PCS2023_Exercises/Projects/Delaunay/Dataset/fileLati.csv";
    CreoFileOutput(outputFilePath, myList);
}


/// PROVA

TEST(TestDelaunay,TestMappa)
{
    Point p1(0.0,0.0);
    Point p2(2.0,1.0);
    Point p3(1.0,-2.0);
    Point p4(3.0,2.0);
    map<int, Point> p;
        p[1] = p1;

        pair<int, Point> elem1 = {2, p2};
        pair<int, Point> elem2 = {3, p3};

        p.insert(elem1);
        p.insert(elem2);
    cout<<p[1].x<<endl;
    for(int i=0; i<int(p.size());i++)
    {}
    std::map<int, char> map = { {1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'} };
        int key = 4;

        map.erase(key);

        for (const auto &entry: map) {
            std::cout << "{" << entry.first << ", " << entry.second << "}" << std::endl;
        }
    //EXPECT_EQ(siintersecano(p1,p2,p3,p4),false);
}

TEST(TestSorting, TestMergeSort)
{
    vector<int> v = {44, 25, 10, 31, 25, 48, 37, 43, 18, 48, 27};
    SortLibrary::MergeSort<int>(v, 0, v.size()-1);
    vector<int> sortedV = {10, 18, 25, 25, 27, 31, 37, 43, 44, 48, 48};
    EXPECT_EQ(v, sortedV);
}

TEST(TestConvexHull, Test1Point)
{
    unsigned int numPoints = 0;
    vector<Point> points = { Point(2, 2, numPoints++) };

    vector<Point> convexHull = ConvexHull(points);

    string convexHullToString;
    ostringstream convert(convexHullToString);

    for(unsigned int i = 0; i < convexHull.size(); i++)
            convert << convexHull[i] << " ";

    string rightConveHull = "0 ";

    EXPECT_EQ(convert.str(), rightConveHull);

}

TEST(TestConvexHull, Test2Points)
{
    unsigned int numPoints = 0;
    vector<Point> points = {Point(2, 2, numPoints++),
                            Point(2.8, 1.7, numPoints++)};

    vector<Point> convexHull = ConvexHull(points);

    string convexHullToString;
    ostringstream convert(convexHullToString);

    for(unsigned int i = 0; i < convexHull.size(); i++)
            convert << convexHull[i] << " ";

    string rightConveHull = "0 1 ";

    EXPECT_EQ(convert.str(), rightConveHull);

}

TEST(TestConvexHull, Test3Points)
{
    unsigned int numPoints = 0;
    vector<Point> points = {Point(2, 2, numPoints++),
                            Point(3, 5, numPoints++),
                            Point(2.8, 1.7, numPoints++)};

    vector<Point> convexHull = ConvexHull(points);

    string convexHullToString;
    ostringstream convert(convexHullToString);

    for(unsigned int i = 0; i < convexHull.size(); i++)
            convert << convexHull[i] << " ";

    string rightConveHull = "0 2 1 ";

    EXPECT_EQ(convert.str(), rightConveHull);

}

TEST(TestConvexHull, TestXPoints)
{
    unsigned int numPoints = 0;
    vector<Point> points = {Point(2, 2, numPoints++),
        Point(2.8, 1.7, numPoints++),
        Point(3, 5, numPoints++),
        Point(7, 3, numPoints++),
        Point(7.2, 5, numPoints++),
        Point(11, 4.3, numPoints++),
        Point(12, 5.2, numPoints++),
        Point(14, 3, numPoints++)
    };

    vector<Point> convexHull = ConvexHull(points);

    string convexHullToString;
    ostringstream convert(convexHullToString);

    for(unsigned int i = 0; i < convexHull.size(); i++)
            convert << convexHull[i] << " ";

    string rightConveHull = "0 1 7 6 2 ";

    EXPECT_EQ(convert.str(), rightConveHull);

}

TEST(TestConvexHull, TestXFloatingPoints)
{
    unsigned int numPoints = 0;
    vector<Point> points = {Point(0.3215348546593775, 0.03629583077160248, numPoints++),
                            Point(0.02402358131857918, -0.2356728797179394, numPoints++),
                            Point(0.04590851212470659, -0.4156409924995536, numPoints++),
                            Point(0.3218384001607433, 0.1379850698988746, numPoints++),
                            Point(0.11506479756447, -0.1059521474930943, numPoints++),
                            Point(0.2622539999543261, -0.29702873322836, numPoints++),
                            Point(-0.161920957418085, -0.4055339716426413, numPoints++),
                            Point(0.1905378631228002, 0.3698601009043493, numPoints++),
                            Point(0.2387090918968516, -0.01629827079949742, numPoints++),
                            Point(0.07495888748668034, -0.1659825110491202, numPoints++),
                            Point(0.3319341836794598, -0.1821814101954749, numPoints++),
                            Point(0.07703635755650362, -0.2499430638271785, numPoints++),
                            Point(0.2069242999022122, -0.2232970760420869, numPoints++),
                            Point(0.04604079532068295, -0.1923573186549892, numPoints++),
                            Point(0.05054295812784038, 0.4754929463150845, numPoints++),
                            Point(-0.3900589168910486, 0.2797829520700341, numPoints++),
                            Point(0.3120693385713448, -0.0506329867529059, numPoints++),
                            Point(0.01138812723698857, 0.4002504701728471, numPoints++),
                            Point(0.009645149586391732, 0.1060251100976254, numPoints++),
                            Point(-0.03597933197019559, 0.2953639456959105, numPoints++),
                            Point(0.1818290866742182, 0.001454397571696298, numPoints++),
                            Point(0.444056063372694, 0.2502497166863175, numPoints++),
                            Point(-0.05301752458607545, -0.06553921621808712, numPoints++),
                            Point(0.4823896228171788, -0.4776170002088109, numPoints++),
                            Point(-0.3089226845734964, -0.06356112199235814, numPoints++),
                            Point(-0.271780741188471, 0.1810810595574612, numPoints++),
                            Point(0.4293626522918815, 0.2980897964891882, numPoints++),
                            Point(-0.004796652127799228, 0.382663812844701, numPoints++),
                            Point(0.430695573269106, -0.2995073500084759, numPoints++),
                            Point(0.1799668387323309, -0.2973467472915973, numPoints++),
                            Point(0.4932166845474547, 0.4928094162538735, numPoints++),
                            Point(-0.3521487911717489, 0.4352656197131292, numPoints++),
                            Point(-0.4907368011686362, 0.1865826865533206, numPoints++),
                            Point(-0.1047924716070224, -0.247073392148198, numPoints++),
                            Point(0.4374961861758457, -0.001606279519951237, numPoints++),
                            Point(0.003256207800708899, -0.2729194320486108, numPoints++),
                            Point(0.04310378203457577, 0.4452604050238248, numPoints++),
                            Point(0.4916198379282093, -0.345391701297268, numPoints++),
                            Point(0.001675087028811806, 0.1531837672490476, numPoints++),
                            Point(-0.4404289572876217, -0.2894855991839297, numPoints++)};

    vector<Point> convexHull = ConvexHull(points);

    string convexHullToString;
    ostringstream convert(convexHullToString);

    for(unsigned int i = 0; i < convexHull.size(); i++)
            convert << convexHull[i] << " ";

    string rightConveHull = "32 39 6 23 37 30 14 31 ";

    EXPECT_EQ(convert.str(), rightConveHull);

}


TEST(TestConvexHull,TestPuntiTriangoloMaxArea)
{
    unsigned int numPoints = 0;
    vector<Point> punti = {Point(3040,4460, numPoints++),
                           Point(4752, 4262, numPoints++),
                           Point(3383, 413, numPoints++),
                           Point(1000,1000, numPoints++),
                           Point(759, 2927, numPoints++),
                           Point(4745, 4322, numPoints++),
                           Point(5000,1000, numPoints++),
                           Point(1213, 691, numPoints++),
                           Point(2506, 4423, numPoints++)};


    vector<Point> convexHull = ConvexHull(punti);
    vector<Point> puntiMaxTriangolo = findMaxTriangle(convexHull);

    string puntiMaxTriangoloToString;
    ostringstream convert(puntiMaxTriangoloToString);

    for(unsigned int i = 0; i < puntiMaxTriangolo.size(); i++)
            convert << puntiMaxTriangolo[i] << " ";

    string rightpuntiMaxTriangolo = "3 6 0 ";

    EXPECT_EQ(convert.str(), rightpuntiMaxTriangolo);

}

TEST(TestConvexHull, TestMaxAreaTriangolo2)
{
    unsigned int numPoints = 0;
    vector<Point> punti = {Point(0.3215348546593775, 0.03629583077160248, numPoints++),
                           Point(0.02402358131857918, -0.2356728797179394, numPoints++),
                           Point(0.04590851212470659, -0.4156409924995536, numPoints++),
                           Point(0.3218384001607433, 0.1379850698988746, numPoints++),
                           Point(0.11506479756447, -0.1059521474930943, numPoints++),
                           Point(0.2622539999543261, -0.29702873322836, numPoints++),
                           Point(-0.161920957418085, -0.4055339716426413, numPoints++),
                           Point(0.1905378631228002, 0.3698601009043493, numPoints++),
                           Point(0.2387090918968516, -0.01629827079949742, numPoints++),
                           Point(0.07495888748668034, -0.1659825110491202, numPoints++),
                           Point(0.3319341836794598, -0.1821814101954749, numPoints++),
                           Point(0.07703635755650362, -0.2499430638271785, numPoints++),
                           Point(0.2069242999022122, -0.2232970760420869, numPoints++),
                           Point(0.04604079532068295, -0.1923573186549892, numPoints++),
                           Point(0.05054295812784038, 0.4754929463150845, numPoints++),
                           Point(-0.3900589168910486, 0.2797829520700341, numPoints++),
                           Point(0.3120693385713448, -0.0506329867529059, numPoints++),
                           Point(0.01138812723698857, 0.4002504701728471, numPoints++),
                           Point(0.009645149586391732, 0.1060251100976254, numPoints++),
                           Point(-0.03597933197019559, 0.2953639456959105, numPoints++),
                           Point(0.1818290866742182, 0.001454397571696298, numPoints++),
                           Point(0.444056063372694, 0.2502497166863175, numPoints++),
                           Point(-0.05301752458607545, -0.06553921621808712, numPoints++),
                           Point(0.4823896228171788, -0.4776170002088109, numPoints++),
                           Point(-0.3089226845734964, -0.06356112199235814, numPoints++),
                           Point(-0.271780741188471, 0.1810810595574612, numPoints++),
                           Point(0.4293626522918815, 0.2980897964891882, numPoints++),
                           Point(-0.004796652127799228, 0.382663812844701, numPoints++),
                           Point(0.430695573269106, -0.2995073500084759, numPoints++),
                           Point(0.1799668387323309, -0.2973467472915973, numPoints++),
                           Point(0.4932166845474547, 0.4928094162538735, numPoints++),
                           Point(-0.3521487911717489, 0.4352656197131292, numPoints++),
                           Point(-0.4907368011686362, 0.1865826865533206, numPoints++),
                           Point(-0.1047924716070224, -0.247073392148198, numPoints++),
                           Point(0.4374961861758457, -0.001606279519951237, numPoints++),
                           Point(0.003256207800708899, -0.2729194320486108, numPoints++),
                           Point(0.04310378203457577, 0.4452604050238248, numPoints++),
                           Point(0.4916198379282093, -0.345391701297268, numPoints++),
                           Point(0.001675087028811806, 0.1531837672490476, numPoints++),
                           Point(-0.4404289572876217, -0.2894855991839297, numPoints++)};

    vector<Point> convexHull = ConvexHull(punti);
    vector<Point> puntiMaxTriangolo = findMaxTriangle(convexHull);

    string puntiMaxTriangoloToString;
    ostringstream convert(puntiMaxTriangoloToString);

    for(unsigned int i = 0; i < puntiMaxTriangolo.size(); i++)
            convert << puntiMaxTriangolo[i] << " ";

    string rightpuntiMaxTriangolo = "32 23 30 ";

    EXPECT_EQ(convert.str(), rightpuntiMaxTriangolo);

}

#endif  //__TEST_CONVEXHULL_H


#endif // __TEST_EMPTY_H
