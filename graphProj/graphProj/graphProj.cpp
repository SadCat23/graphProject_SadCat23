// graphProj.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include "Engine.h"

#include <iostream>


int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Engine engine;
	engine.Initialize(hInstance, "CompGraph_SadCat", "CompGraph_SadCat", 1500, 800, 0, 0);
	
	

	XMFLOAT3 yellow = XMFLOAT3(0.9f, 0.4f, 0.05f);
	Model* sun = engine.AddModel(XMFLOAT3(0.0f, 0.0f, 0.0f), nullptr, yellow);
	sun->SetScale(2.5);

	////////////////////////
	XMFLOAT3 green = XMFLOAT3(0.5f, 0.8f, 0.15f);
	Model* planet1=engine.AddModel(XMFLOAT3(8.0f, 0.0f, 0.0f), sun, green);
	planet1->SetScale(1);

	XMFLOAT3 gray = XMFLOAT3(0.7f, 0.7f, 0.7f);
	Model* moon11 =engine.AddModel(XMFLOAT3(2.0f, 0.0f, 0.0f), planet1, gray);
	moon11->SetScale(0.3);
	Model* moon12 = engine.AddModel(XMFLOAT3(1.2f, 0.0f, 0.0f), planet1, gray);
	moon12->SetScale(0.2);


	//////////////////////////
	XMFLOAT3 planetColor1 = XMFLOAT3(0.9f, 0.0f, 0.0f);
	Model* planet2 = engine.AddModel(XMFLOAT3(4.0f, 0.0f, 0.0f), sun, planetColor1);
	planet2->SetScale(0.5);

	XMFLOAT3 moonColor1 = XMFLOAT3(0.2f, 0.2f, 0.2f);
	Model* moon21 = engine.AddModel(XMFLOAT3(1.0f, 0.0f, 0.0f), planet2, moonColor1);
	moon21->SetScale(0.1);
	/////////////////////////

	XMFLOAT3 planetColor2 = XMFLOAT3(0.4f, 0.9f, 0.9f);
	Model* planet3 = engine.AddModel(XMFLOAT3(14.0f, 0.0f, 0.0f), sun, planetColor2);
	planet3->SetScale(1.5);

	Model* moon31 = engine.AddModel(XMFLOAT3(2.0f, 2.0f, 0.0f), planet3, gray);
	moon31->SetScale(0.7);
	
	Model* moon32 = engine.AddModel(XMFLOAT3(1.0f, 0.0f, 0.0f), moon31, moonColor1);
	moon32->SetScale(0.2);



	while (engine.ProcessMessage()==true)
	{	
		engine.Update();
		engine.RenderFrame();
	}



	return 0;
  
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
