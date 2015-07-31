#include <stdio.h>
#include <math.h>

#include "vtkActor.h"
#include "vtkSmartPointer.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkIdList.h"
#include "vtkPolyData.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkPolyDataMapper.h"

class O3DPoint
{
	public:
		O3DPoint(double inX, double inY, double inZ)
		{
			x = inX;
			y = inY;
			z = inZ;
		}
		void assign(O3DPoint *A)
		{
			x = A->x;
			y = A->y;
			z = A->z;
		}
		void PointSum(O3DPoint *B, O3DPoint *C)
		{
			x = C->x + B->x;
			y = C->y + B->y;
			z = C->z + B->z;
		}

	public:
		double x;
		double y;
		double z;
};


void DivideTriangle(O3DPoint *p1, O3DPoint *p2, O3DPoint *p3, int steplength, vtkSmartPointer<vtkPoints> surfacePoints, vtkSmartPointer<vtkCellArray> surfacepoly);
void funcprint(O3DPoint *A, O3DPoint *B, O3DPoint *C);



int main()
{
	vtkSmartPointer<vtkPolyData> data = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPoints> surfacePoints = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> surfacePoly = vtkSmartPointer<vtkCellArray>::New();


	O3DPoint A(0,0,1), B(0,2,2), C(2,0,3);
	
	DivideTriangle(&A, &B, &C, 1, surfacePoints, surfacePoly);
	
	data->SetPoints(surfacePoints);
	data->SetPolys(surfacePoly);

	vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper1->SetInputData(data);
	vtkSmartPointer<vtkActor> actor1 = vtkSmartPointer<vtkActor>::New();
	actor1->SetMapper(mapper1);
	actor1->AddPosition(5, 5, 0);

	vtkRenderer *renderer = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);
	renderer->AddActor(actor1);
	renderer->SetBackground(1, 1, 1);

	renWin->SetSize(300, 300);
	renWin->Render();
	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//设置平台独立响应的鼠标键盘时钟交互机制
	iren->SetRenderWindow(renWin);
	//设置交互器的渲染窗口，消息是通过渲染窗口捕获的
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	iren->SetInteractorStyle(style);
	//定义交互器样式
	iren->Initialize();
	//开始前需要调用初始化
	iren->Start();
	//交互器处于等待状态

	return 0;
}

void DivideTriangle(O3DPoint *p1, O3DPoint *p2, O3DPoint *p3, int steplength, vtkSmartPointer<vtkPoints> surfacePoints, vtkSmartPointer<vtkCellArray> surfacepoly)
{
	///////////////////////////////////
	//                top            //
	//                / \            //
	//               /   \           //
	//              /     \          //
	//             /       \         //
	//      bottom1---------bottom2  //
	///////////////////////////////////

	O3DPoint *Pbottom1 = new O3DPoint(0, 0, 0), *Pbottom2 = new O3DPoint(0, 0, 0), *Ptop = new O3DPoint(0, 0, 0), *Pbottom1next = new O3DPoint(0, 0, 0), *Pbottom2next = new O3DPoint(0, 0, 0), *A = new O3DPoint(0, 0, 0), *B = new O3DPoint(0, 0, 0), *C = new O3DPoint(0, 0, 0), *D = new O3DPoint(0, 0, 0);


	//getbottom,the longest line,for the shortest iteration times
	float p1p2, p1p3, p2p3, topb1;
	p1p2 = (p1->x - p2->x)*(p1->x - p2->x) + (p1->y - p2->y)*(p1->y - p2->y) + (p1->z - p2->z)*(p1->z - p2->z);
	p1p2 = sqrtf(p1p2);
	p1p3 = (p1->x - p3->x)*(p1->x - p3->x) + (p1->y - p3->y)*(p1->y - p3->y) + (p1->z - p3->z)*(p1->z - p3->z);
	p1p3 = sqrtf(p1p3);
	p2p3 = (p2->x - p3->x)*(p2->x - p3->x) + (p2->y - p3->y)*(p2->y - p3->y) + (p2->z - p3->z)*(p2->z - p3->z);
	p2p3 = sqrtf(p2p3);
	if ((p1p2 >= p1p3) && (p1p2 >= p2p3)){
		Pbottom1->assign(p1);
		Pbottom2->assign(p2);
		Ptop->assign(p3);
		topb1 = p1p3;
	}
	else if ((p1p3 >= p1p2) && (p1p3 >= p2p3)){
		Pbottom1->assign(p1);
		Pbottom2->assign(p3);
		Ptop->assign(p2);
		topb1 = p1p2;
	}
	else{
		Pbottom1->assign(p2);
		Pbottom2->assign(p3);
		Ptop->assign(p1);
		topb1 = p1p2;
	}

	//divide times
	int k = topb1 / steplength + 0.5;

	// step from current bottom to next bottom
	O3DPoint *b1step = new O3DPoint(0, 0, 0);
	O3DPoint *b2step = new O3DPoint(0, 0, 0);
	float stepratio = (float)1 / k;
	b1step->x = stepratio * (Ptop->x - Pbottom1->x);
	b1step->y = stepratio * (Ptop->y - Pbottom1->y);
	b1step->z = stepratio * (Ptop->z - Pbottom1->z);
	b2step->x = stepratio * (Ptop->x - Pbottom2->x);
	b2step->y = stepratio * (Ptop->y - Pbottom2->y);
	b2step->z = stepratio * (Ptop->z - Pbottom2->z);

	//       A
	//       |\
	//       | \
	//       B---C(A')
	//        \  |\
	//         \ | \
	//        D(B')-C'(B'')                                         bottom1(A)                 bottom2(B)-top(C)
	// A + b1step = B                                                   |\                               \  |
	// A + stepAC = C   or A+b1step-b2step = C                          | \                               \ |
	// B + stepAC = D                                             top(B)-bottom2(C)                     bottom1(D)
	O3DPoint *stepAC = new O3DPoint(0,0,0);
	stepAC->x = stepratio * (Pbottom2->x - Pbottom1->x);
	stepAC->y = stepratio * (Pbottom2->y - Pbottom1->y);
	stepAC->z = stepratio * (Pbottom2->z - Pbottom1->z);

	//divide from the bottom to top
	for (int i = 0; i < k - 1; i++){
		// tell which layer it is now and how many triangles in this layer
		printf("Layer %d contains %d triangles\n", k-i, (2*k-2*i-1));
		//divide from Pbottom1 and Pbottom1next
		//             Pbottom1 
		//                ^
		//                |\
		//                | \
		//   Pbottom1next ^\ \
		//                  \ \
		//                   \ \
		//                    \_\
		//        Pbottom2next^  ^
		//                       Pbottom2

		//get Pbottomnext at the beginning of the loop
		Pbottom1next->PointSum(Pbottom1, b1step);
		Pbottom2next->PointSum(Pbottom2, b2step);
		
		//process the first triangle
		A->assign(Pbottom1);
		B->assign(Pbottom1next);
		C->PointSum(A, stepAC);
		
		//save B(top) A(bottom1) C(bottom2) and show
		printf("triangle no: 1\n");
		funcprint(B, A, C);
		int a, b, c;
		vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();
		a = surfacePoints->InsertNextPoint(B->x,B->y,B->z);
		b = surfacePoints->InsertNextPoint(A->x,A->y,A->z);
		c = surfacePoints->InsertNextPoint(C->x,C->y,C->z);
		idList->InsertNextId(a);
		idList->InsertNextId(b);
		idList->InsertNextId(c);
		surfacepoly->InsertNextCell(idList);
		
		//process following triangles in this layer
		for (int j = 1; j < (1 + 2 * (k - i - 1)); j++){
			if (j % 2 == 0){
				A->assign(C);
				B->assign(D);
				C->PointSum(A, stepAC);
				//save B(top) A(bottom1) C(bottom2)
				printf("triangle no: %d\n",j+1);
				funcprint(B, A, C);
				int a, b, c;
				vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();
				a = surfacePoints->InsertNextPoint(B->x, B->y, B->z);
				b = surfacePoints->InsertNextPoint(A->x, A->y, A->z);
				c = surfacePoints->InsertNextPoint(C->x, C->y, C->z);
				idList->InsertNextId(a);
				idList->InsertNextId(b);
				idList->InsertNextId(c);
				surfacepoly->InsertNextCell(idList);
			}
			else{
				D->PointSum(B,stepAC);
				//save C(top) D(bottom1) B(bottom2)
				printf("triangle no: %d\n", j + 1);
				funcprint(C, D, B);
				int a, b, c;
				vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();
				a = surfacePoints->InsertNextPoint(C->x, C->y, C->z);
				b = surfacePoints->InsertNextPoint(D->x, D->y, D->z);
				c = surfacePoints->InsertNextPoint(B->x, B->y, B->z);
				idList->InsertNextId(a);
				idList->InsertNextId(b);
				idList->InsertNextId(c);
				surfacepoly->InsertNextCell(idList);
			}
		}
		//change the value of Pbottom at the end of loop for the next loop
		Pbottom1->assign(Pbottom1next);
		Pbottom2->assign(Pbottom2next);
	}

	//the last single triangle
	printf("Layer 1 contains 1 triangle\n");
	funcprint(Ptop, Pbottom1, Pbottom2);
	int a, b, c;
	vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();
	a = surfacePoints->InsertNextPoint(Ptop->x, Ptop->y, Ptop->z);
	b = surfacePoints->InsertNextPoint(Pbottom1->x, Pbottom1->y, Pbottom1->z);
	c = surfacePoints->InsertNextPoint(Pbottom2->x, Pbottom2->y, Pbottom2->z);
	idList->InsertNextId(a);
	idList->InsertNextId(b);
	idList->InsertNextId(c);
	surfacepoly->InsertNextCell(idList);

}

void funcprint(O3DPoint *A, O3DPoint *B, O3DPoint *C)
{
	printf("(%.3f %.3f %.3f)\n", A->x, A->y, A->z);
	printf("(%.3f %.3f %.3f)\n", B->x, B->y, B->z);
	printf("(%.3f %.3f %.3f)\n", C->x, C->y, C->z);
	printf("\n");
}
