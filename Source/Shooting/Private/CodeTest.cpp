// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeTest.h"

#pragma warning (disable: 4458)	// c4458 경고 끄기

// 기본 값을 설정할 수 있다.
// 생성자 함수
ACodeTest::ACodeTest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	age = 42;
	height = 180;
}

// Called when the game starts or when spawned
void ACodeTest::BeginPlay()
{
	Super::BeginPlay();

#pragma region 사칙연산 

	//// Add 함수에 입력값 8, 4를 넣어서 결과를 result 변수에 받는다.
	//Add(8, 4);
	//int32 result2 = Subtract(8, 4);
	//int32 result3 = Multiply(8, 4);
	//float result4 = Divide(4, 8);

	//// result 변수에 있는 값을 화면에 출력한다.
	////UE_LOG(LogTemp, Warning, TEXT("Result1 : %d"), result1);
	//UE_LOG(LogTemp, Warning, TEXT("Result2 : %d"), result2);
	//UE_LOG(LogTemp, Warning, TEXT("Result3 : %d"), result3);
	//UE_LOG(LogTemp, Warning, TEXT("Result4 : %.1f"), result4);


	//// 만일, 나이가 30살 이상이면 "할배"라고 출력하고 싶다.
	//if (age >= 30)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("할배다~"));
	//}
	//// 그렇지 않고 만일, 나이가 20살 이상이면 "꼰대"라고 출력하고 싶다.
	//else if (age >= 20)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("꼰대시군요~"));
	//}
	//// 그렇지 않다면, "꼬맹이"라고 출력하고 싶다.
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("잼민이네~"));
	//}
#pragma endregion

#pragma region 조건식



	//// 만일 나이가 20살 이상이면서, 키가 160 이상인 경우에는
	//if (age >= 20 || height >= 160)
	//{
	//	// "환영"이라고 출력한다.
	//	UE_LOG(LogTemp, Warning, TEXT("환영합니다~~~!!"));
	//}
	//// 그렇지 않으면,
	//else
	//{
	//	// "외면"이라고 출력한다.
	//	UE_LOG(LogTemp, Warning, TEXT("실례했습니다."));
	//}

	// 국적이 "한국"이면서, 나이가 20살 이상 40세 미만일 때,
	//if (nation == TEXT("한국"))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("한국인"));
	//	
	//	if (age >= 20 && age < 40)
	//	{
	//		// "청년"이라고 출력한다.
	//		UE_LOG(LogTemp, Warning, TEXT("청년!"));
	//	}
	//}
	//// 그렇지 않으면,
	//else
	//{
	//	// "청년이 아니다"라고 출력한다.
	//	UE_LOG(LogTemp, Warning, TEXT("청년이 아니시군요..."));
	//}

	/*FString result = StringAppender(TEXT("박원석"), TEXT("멋쟁이"));
	UE_LOG(LogTemp, Warning, TEXT("함수 실행 결과: %s"), *result);*/

	//int32 checker = 123;

	//// 만일, checker 변수의 값이 짝수라면
	//if (checker % 2 == 0)
	//{
	//	// "even"이라고 출력한다.
	//	UE_LOG(LogTemp, Warning, TEXT("even"));
	//}
	//// 그렇지 않고, 변수의 값이 홀수라면
	//else
	//{
	//	// "odd"라고 출력한다.
	//	UE_LOG(LogTemp, Warning, TEXT("odd"));
	//}

#pragma endregion

#pragma region 반복문

	//UE_LOG(LogTemp, Warning, TEXT("%d"), Add(3, 1));

	//// 반복문
	//// "안녕하세요"라는 문자열을 10번 출력한다.
	////MulitplicationTable(7);

	//for (int32 i = 2; i < 10; i++)
	//{
	//	for (int32 j = 1; j < 10; j++)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("%d x %d = %d"), i, j, i * j);
	//	}
	//	UE_LOG(LogTemp, Warning, TEXT("==================================="));
	//}
#pragma endregion


#pragma region Array, Map

	//distances.Add("Seoul", 250.5f);
	//distances.Add("Incheon", 250.5f);
	//distances.Add("SungNam", 135.5f);

	//FString myKey = "Incheon";
	//UE_LOG(LogTemp, Warning, TEXT("%s : %f"), *myKey, distances[myKey]);

	//for (auto dist : distances)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%s - %f"), *dist.Key, dist.Value)
	//}


	//UE_LOG(LogTemp, Warning, TEXT("Array Length %d"), ages.Num());

	//for (int32 i = 0; i < ages.Num(); i++)
	//for(int32 age : ages)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%d"), age);
	//}
#pragma endregion

	//int32 number1 = 10;
	//int32 copyNum = number1;
	//number1 = 30;

	//UE_LOG(LogTemp, Warning, TEXT("number1: %d, copyNum: %d"), number1, copyNum);


	//int32 number1 = 10;
	//int32* copyNum = &number1;
	//number1 = 30;

	//UE_LOG(LogTemp, Warning, TEXT("number1: %p, copyNum: %p"), &number1, copyNum);

	int32 aa = 10;
	int32 bb = 15;
	int32 result;
	result = Add(aa, bb);

	UE_LOG(LogTemp, Warning, TEXT("Result = %d, aa = %d, bb = %d"), result, aa, bb);
}

void ACodeTest::MulitplicationTable(int32 guguNum)
{
	for (int32 i = 1; i < 10; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d x %d = %d"), guguNum, i, guguNum * i);
	}
}

// Called every frame
void ACodeTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ACodeTest::Add(int32 &num1, int32 &num2)
{
	// to do
	num1 *= 2;
	num2 *= 2;

	int32 result = num1 + num2;
	return result;
}

int32 ACodeTest::Subtract(int32 num1, int32 num2)
{
	return num1 - num2;
}

int32 ACodeTest::Multiply(int32 num1, int32 num2)
{
	return num1 * num2;
}

float ACodeTest::Divide(int32 num1, int32 num2)
{
	return (float)num1 / (float)num2;
}

FString ACodeTest::StringAppender(FString a, FString b)
{
	return a + b;
}