#ifndef __TEST_EMPTY_H
#define __TEST_EMPTY_H



#include <gtest/gtest.h>
#include "empty_class.hpp"
#include "funzioni_triangoli.hpp"
#include "funzioni.hpp"
#include "ipotesi_e_flip.hpp"
#include "convex_hull.hpp"


#include "iostream"
#include "sorting.hpp"
#include "vector"



using namespace testing;
using namespace Delaunay;
using namespace std;




TEST(TestDelaunay, TestOrdinaPunti)
{

    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,0.0);

    Triangle Triangolo(p1,p2,p3);
    Triangle TriangoloOrdinato(p1,p3,p2);

    EXPECT_EQ(Triangolo, TriangoloOrdinato);
}


//TEST FUNZIONI TRIANGOLI

TEST(TestDelaunay,TestDentro)
{
    Point p1(0.0,0.0);
    Point p2(1.0,1.0);
    Point psx(0.333,1.0);

    EXPECT_TRUE(dentro(p1,p2,psx));
}

TEST(TestDelaunay,TestFuori)
{
    Point p1(0.0,0.0);
    Point p2(1.0,1.0);
    Point psx(1.0,0.3333);

    EXPECT_FALSE(dentro(p1,p2,psx));
}

TEST(TestDelaunay,TestIsInsideCircumCircle)
{
    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,0.0);
    Point pbetween(0.6,0.6);
    Triangle TriangoloOrdinato(p1,p3,p2);

    EXPECT_TRUE(isInsideCircumcircle(TriangoloOrdinato,pbetween));
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


TEST(TestDelaunay,TestTrovaId)
{
    Point p1(0.0,0.0);
    Point p2(1.0,0.0);
    Point p3(0.0,1.0);
    Triangle abc (p1,p2,p3);
    abc.id=2;
    Point p4(2.0, 1.0);
    Triangle cbd (p3,p2,p4);
    cbd.id = 4;
    Point p5(10.0,10.0);
    Point p6(11.0,11.0);
    Point p7(12.0,12.0);
    Triangle def (p5,p6,p7);
    def.id = 5;
    vector<Triangle> ListaTriangoliOutput;
    ListaTriangoliOutput.push_back(abc);
    ListaTriangoliOutput.push_back(cbd);
    ListaTriangoliOutput.push_back(def);


    EXPECT_EQ(trova_id(ListaTriangoliOutput, 2), 0);
}


TEST(TestDelaunay,TestTriangoliInt)
{
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

TEST(TestDelaunay,TestPosizione)
{
    Point p1(0.0,0.0);
    Point p2(1.0,0.0);
    Point p3(0.0,1.0);
    Triangle abc (p1,p2,p3);
    abc.id=2;
    Point p4(2.0, 1.0);
    Triangle cbd (p3,p2,p4);
    cbd.id = 4;
    Point p5(10.0,10.0);
    Point p6(11.0,11.0);
    Point p7(12.0,12.0);
    Triangle def (p5,p6,p7);
    def.id = 5;
    Point p8(100.0,100.0);
    vector<Triangle> ListaTriangoliOutput;
    ListaTriangoliOutput.push_back(abc);
    ListaTriangoliOutput.push_back(cbd);
    ListaTriangoliOutput.push_back(def);
    int a = 4;

    EXPECT_EQ(Posizione(p8,ListaTriangoliOutput,a), false);
}

TEST(TestDelaunay, TestAggiorna_Adiacenze)
{

//    tu fai flip e hai due triangoli vecchi e due nuovi
//    vuoi trovare tra vecchio e nuovo il lato in comune e salvare nell'adiacenza del triangolo nuovo, nella posizione giusta,
//    l'adiacenza che aveva il triangolo vecchio in quella posizione,
//    quindi per il test devi definire l'adiacenza nel triangolo old e vedere che quell'adiacenza la salvi in triangolo nuovo


    Point p1(0.0,1.0,1);
    Point p4(1.0,0.0,4);
    Point p3(2.0,1.0,3);
    Point p2(2.0,2.0,2);

    Lati l1 =Lati(p3,p2,1);  // lato adiacente t1 t2
    Lati l2 =Lati(p2,p1,2);
    Lati l3 =Lati(p1,p3,3);
    Lati l4 =Lati(p4,p3,4);
    Lati l5 =Lati(p2,p4,5);

    Triangle old = Triangle(l1,l2,l3);
    old.id = 1;
    old.adiacenti[0] = 3  ;    // vogliamo che in new, nella posizione del lato l1 (che è quello adiacente con old), si conservi l'adiacenza di old con il triangolo 3
    Triangle nuovo = Triangle(l4,l1,l5);
    nuovo.id = 2;

    vector<Triangle> triangoli;
    triangoli.push_back(old);
    triangoli.push_back(nuovo);

    aggiorna_adiacenza(old,nuovo,triangoli);
    // Verifica del risultato
    //cout << "conservazione di 3 adiacente a old in nuovo: " << nuovo.adiacenti[0] << ", " << nuovo.adiacenti[1] << ", " << nuovo.adiacenti[2] << std::endl;

    EXPECT_EQ(nuovo.adiacenti[1],3);
}

TEST(TestDelaunay,TestVerifica_adiacenza)
{
    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,1.0);
    Point p4(2.0,0.0);
    Triangle t1 = Triangle(p1,p2,p3);
    Triangle t2 = Triangle(p1,p4,p3);
    Lati l;
    bool a = verifica_adiacenza(t1,t2,l);
    EXPECT_EQ(a,true);
}

TEST(TestDelaunay, TestTrovaNuovoLato)
{
    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,1.0);
    Point p4(2.0,0.0);
    Triangle t1 = Triangle(p1,p2,p3);
    Triangle t2 = Triangle(p1,p4,p3);
    Lati oldlato = Lati(p2,p3,1);
    Lati l1 = Lati(p1,p4,2);
    Lati lato;
    int id;
    trova_nuovo_lato(t1, t2, oldlato,lato, id);
    EXPECT_EQ(l1, lato);

}





// TEST FUNZIONI

TEST(TestDelaunay, TestInput)
{
    vector <Point> points;
    string InputFilePath = "C:/Users/sarab/OneDrive/Desktop/PCS2023_Exercises/Projects/Delaunay/Dataset/Test2.csv";
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

    InputFile(InputFilePath,points);
    EXPECT_EQ(points,punti);
}

TEST(TestDelaunay, TestFileOutput)
{
    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,0.0);
    Point p4(1.0,1.0);
    Point p5(2.0,2.0);
    Point p6(3.4,3.1);
    vector<Lati> myList{Lati(p1, p2), Lati(p3, p4), Lati(p5, p6)};
    string outputFilePath = "C:/Users/sarab/OneDrive/Desktop/PCS2023_Exercises/Projects/Delaunay/Dataset/fileLati.csv";
    CreoFileOutput(outputFilePath, myList);
}


// TEST IPOTESI E FLIP

TEST(TestDelaunay, TestFlip)
{
    vector<Lati> lati;
    vector<Triangle> ListaTriangoli;
    Point p1(0.0,0.0,0);
    Point p2(1.0,0.0,1);
    Point p3(0.0,1.0,2);
    Triangle ABC (p1,p2,p3,0,0);
    Point p4(2.0, 1.0,3);
    Triangle CBD (p3,p2,p4,1,3);
    ListaTriangoli.push_back(ABC);
    ListaTriangoli.push_back(CBD);
    Lati l;
    verifica_adiacenza(ABC,CBD,l);
    int id = 0;
    Flip(ABC, CBD,l,ListaTriangoli,lati,id,id,p1);
    Triangle ADB (p1,p4,p2,2,8);
    Triangle ADC (p1,p4,p3,3,11);
    EXPECT_EQ(l,Lati(p2,p3));
}

TEST(TestDelaunay, TestIpotesi_int)
{
    vector<Point> punti = {Point(0.0,0.0),Point(0.3,1.0),Point(1.0,1.0),Point(2.0,2.0),Point(0.5,2.0),Point(1.1,1.1),Point(0.7,4.0),Point(4.0,0.0),Point(2.2,1.1),Point(0.2,1.4)};
    vector<Lati> lati;
    int t_id;
    int l_id;
    vector<Point> puntiConvexHull = ConvexHull(punti);
    vector<Triangle> triangoli = TriangoliConvexHull(puntiConvexHull,lati,t_id,l_id);
    Point p1(0.9,0.5);
    vector<Lati> lati_ipotesi;
    int t_id_ipotesi;
    int l_id_ipotesi;
    Triangle t;
    ipotesi_int(p1,t, triangoli, lati_ipotesi, t_id_ipotesi, l_id_ipotesi);
    int num = triangoli.size();
    int count = 0;
    for (int i=0; i<num; i++)
    {
        bool a = isInsideCircumcircle(triangoli[i],p1);
        if (a)
            count++;
    }
    EXPECT_EQ(num,count);
}

#endif // __TEST_EMPTY_H




//TEST CONVEXHULL
#ifndef __TEST_CONVEXHULL_H
#define __TEST_CONVEXHULL_H
using namespace testing;
using namespace Delaunay;
using namespace std;
using namespace SortLibrary;



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




#endif  //__TEST_CONVEXHULL_H
