#include "..\Public\NonPlayerActorBase.h"
#include "..\Public\NonPlayerActorBase.h"
#include "..\Public\NonPlayerActorBase.h"
#include "..\Public\NonPlayerActorBase.h"
// The game and source code belongs to Team 7 Programming team


#include "NonPlayerActorBase.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "EnemySpawner.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "PxMathUtils.h"
#include "DrawDebugHelpers.h"
#include "../Mina.h"


// Sets default values
ANonPlayerActorBase::ANonPlayerActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the collider for the shroob
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	BoxCollider->InitBoxExtent(FVector(50.f));

	// Set OurCollider to be the RootComponent
	RootComponent = BoxCollider;

	//Sensing Sphere. checking if there is a player near by
	SensingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ShroobSenesingSphere"));
	SensingSphere->SetupAttachment(GetRootComponent());

	// Set up our visible mesh
	VisibleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShroobVisibleMesh"));
	VisibleMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ANonPlayerActorBase::BeginPlay()
{
	Super::BeginPlay();

	//player = Cast<AActor>(GetWorld()->GetFirstPlayerController());
	//if (!mina)
	//	mina = GetWorld()->GetFirstPlayerController();
	//if (mina)
	//	player = Cast<AActor>(mina);
	player = Cast<AActor>(GetWorld()->GetFirstPlayerController()->GetPawn());

	SensingSphere->OnComponentBeginOverlap.AddDynamic(this, &ANonPlayerActorBase::onOverlap);
	SensingSphere->OnComponentEndOverlap.AddDynamic(this, &ANonPlayerActorBase::endOverlap);
	

	/* Finds area actor to move across */
	if (movArea) {
		movAreaActor = Cast<AActor>(movArea);
		MoveAreaVector = movAreaActor->GetActorLocation();
	}
	FVector Forward = GetActorForwardVector();

	/* For å vise vision til Actor */
	//Line = FVector(100, 0, 0) * Forward;
	//FVector OldLine = Line;
	//Line.X = cos(45) * OldLine.X - sin(45) * OldLine.Y;
	//Line.Y = sin(45) * OldLine.X + cos(45) * OldLine.Y;
}

// Called every frame
void ANonPlayerActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	/* For å vise vision til Actor */
	/* Den roterer, men det skjer alt for fort */
	//float angle = Private_Rotation_Z * (PI/180);
	//FVector OldLine = Line;
	//Line.X = cos(angle) * OldLine.X - sin(angle) * OldLine.Y;
	//Line.Y = sin(angle) * OldLine.X + cos(angle) * OldLine.Y;
	//DrawDebugLine(
	//	GetWorld(),
	//	GetActorLocation(),
	//	GetActorLocation() + Line,
	//	FColor(0, 255, 0),
	//	false,
	//	0,
	//	0,
	//	1.f
	//);

	PlayerLocation = player->GetActorLocation();
	//UE_LOG(LogTemp, Display, TEXT("Player: %s"), *PlayerLocation.ToString());

	ShowStateColor();
}

void ANonPlayerActorBase::ShowStateColor()
{
	/* Punkt over actor som skal vise hvilken state den er i */
	DrawDebugLine(
		GetWorld(),
		GetActorLocation() + FVector(0, 0, 100),
		GetActorLocation() + FVector(0, 0, 100),
		StateColor,
		false,
		0,
		0,
		25.f
	);
}

void ANonPlayerActorBase::DrawDebugLineBetweenActors(FVector OtherActor, FColor color)
{
	DrawDebugLine(
		GetWorld(),
		GetActorLocation(),
		OtherActor,
		color,
		false,
		0.f,
		0,
		1.f
	);
}

FHitResult ANonPlayerActorBase::RayTracer(float range)
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, this);

	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		this->GetActorLocation(),
		this->GetActorLocation() - FVector(0, 0, range),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel2),
		TraceParams
	);
	return Hit;
}

void ANonPlayerActorBase::MoveAreaCheck(float range)
{
	FHitResult HitResult = RayTracer(range);

	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit) {
		//UE_LOG(LogTemp, Display, TEXT("%s"), *ActorHit->GetName());
		if (ActorHit == movAreaActor) {
			/* Actor is over its movement area */
			Private_Rotation_Z = 0;
			bRotateBack = false;
			//InitializeRotation = 0;
		}
		else {
		}
	}
	else {
		/* Actor is NOT over its movement area */
		Private_Rotation_Z = Rotation_Z;
		bRotateBack = true;
	}
}

/* Currently only rotates towards origo of MoveAreaVector */
void ANonPlayerActorBase::RotateToVector()
{
	/* Randomizes MoveAreaVector to give some variation */
	FVector MoveToVector = MoveAreaVector + FVector(FMath::RandRange(
		-RotationPointRandomRange, RotationPointRandomRange),
		FMath::RandRange(-RotationPointRandomRange, RotationPointRandomRange), 0);

	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();
	FVector Current = GetActorLocation();
	FVector Turn = MoveToVector - Current;

	float dotProduct_Forward = dotProduct2D(Forward, Turn);
	dotProduct_Right = dotProduct2D(Right, Turn);

	float rotationStrength = (
		(acosf(dotProduct_Forward) * (180 / PI)) /
		180
		);

	Private_Rotation_Z = Rotation_Z * rotationStrength;
	if (dotProduct_Forward < 1) {
		if (dotProduct_Right >= 0) {
			AddActorLocalRotation(FRotator(0, Private_Rotation_Z, 0));
		}
		else if (dotProduct_Right < 0) {
			Private_Rotation_Z = -Private_Rotation_Z;
			AddActorLocalRotation(FRotator(0, Private_Rotation_Z, 0));
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("Angle: %f"), Private_Rotation_Z);
}

float ANonPlayerActorBase::dotProduct2D(FVector vec1, FVector vec2)
{
	float a = vec1.X * vec2.X + vec1.Y * vec2.Y;
	float b = FGenericPlatformMath::Sqrt(
		FGenericPlatformMath::Pow(vec1.X, 2) +
		FGenericPlatformMath::Pow(vec1.Y, 2)
	);
	float c = FGenericPlatformMath::Sqrt(
		FGenericPlatformMath::Pow(vec2.X, 2) +
		FGenericPlatformMath::Pow(vec2.Y, 2)
	);

	return (a / (b * c));
}

void ANonPlayerActorBase::DetectPlayer(float deltatime)
{
	if (States == IDLE || States == AWAREOFPLAYER) {

		FVector ThisActor = GetActorLocation();
		//FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		FVector vec = PlayerLocation - ThisActor;

		float DotProd = dotProduct2D(GetActorForwardVector(), vec);
		float angle = acosf(DotProd) * (180 / PI);
		//float length = VectorMagnitude(vec);


		if (angle < DetectionAngle / 2 && OtherActorWithinReach(PlayerLocation, DetectionLength)) {
			TimeDetected += deltatime;
			//UE_LOG(LogTemp, Warning, TEXT("Time detected: %f"), TimeDetected);

			DrawDebugLineBetweenActors(PlayerLocation, StateColor);

			if (TimeDetected > DetectionTimer) {
				//UE_LOG(LogTemp, Warning, TEXT("Player detected"));
				//UE_LOG(LogTemp, Warning, TEXT("%s is shocked"), *GetName());
				States = SHOCK;
				TimeDetected = 0;
			}
			//bPlayerDetection = true;
		}
		else {
			//UE_LOG(LogTemp, Error, TEXT("Cant see player"));
			TimeDetected = 0;
			//bPlayerDetection = false;
		}

		//UE_LOG(LogTemp, Warning, TEXT("Length from Shroob to player: %f"), length);
		//UE_LOG(LogTemp, Warning, TEXT("Angle from forward vec to player: %f"), angle);
	}
	if (States == HOSTILE) {
		/* Hvis spilleren kommer seg langt nok unna og holder seg unna i noen sekunder
		*	så går actor over til AWAREOFPLAYER */
	}
	if (States == AWAREOFPLAYER) {
		/* Er spilleren lenge nok borte fra actor så går den tilbake til IDLE 
		*	Hvis ikke så blir den hostile igjen */
	}
}

bool ANonPlayerActorBase::OtherActorWithinReach(FVector OtherActor, float reach)
{
	FVector Self = GetActorLocation();
	FVector vec = OtherActor - Self;
	float length = VectorMagnitude(vec);
	return length < reach;
}

void ANonPlayerActorBase::ActorState(float deltatime)
{
	switch (States) {
	case IDLE:	// Idle
		/* I IDLE så skal den bare bevege seg rundt på  platformen sin. Vil gjøre checks for å 
		*	se om den kan legge merke til spilleren. */
		break;
	case SHOCK:	// Shocked
		/* Skal gjøre en sjekk når den entrer SHOCK for å se om spilleren er der.
		*	Etter at shock timeren har gått ut så gjør den en til sjekk,
		*	hvis spilleren da fortsatt er der, så går den over til HOSTILE
		*	Hvis ikke så går den til enten IDLE eller AWAREOFPLAYER */
		break;
	case HOSTILE:	// Hostile
		/* Hvis spilleren kommer seg langt nok unna og holder seg unna i noen sekunder
		*	så går actor over til AWAREOFPLAYER */
		break;
	case AWAREOFPLAYER:	// Aware of player
		/* Er spilleren lenge nok borte fra actor så går den tilbake til IDLE
		*	Kommer spilleren tilbake går den tilbake til HOSTILE */
		break;
	default:
		break;
	}
}

void ANonPlayerActorBase::ActorIDLE(float deltatime)
{
}

void ANonPlayerActorBase::ActorSHOCK(float deltatime)
{
}

void ANonPlayerActorBase::ActorHOSTILE(float deltatime)
{
}

void ANonPlayerActorBase::ActorAWAREOFPLAYER(float deltatime)
{
}

// What happens on this actor being destroyed
void ANonPlayerActorBase::HandleDestruction()
{
}

void ANonPlayerActorBase::onOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == player){
		//UE_LOG(LogTemp, Warning, TEXT("Player WITHIN sensingsphere"));
		if (States == IDLE) {
			UE_LOG(LogTemp, Display, TEXT("%s is Shocked"), *GetName());
			States = SHOCK;
		}
		if (States == AWAREOFPLAYER) {
			States = HOSTILE;
		}
	}
}

void ANonPlayerActorBase::endOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor == player) {
		//UE_LOG(LogTemp, Warning, TEXT("Player OUTSIDE sensingsphere"));
	}
}

float ANonPlayerActorBase::VectorMagnitude(FVector vec)
{
	return sqrt(vec.X * vec.X + vec.Y * vec.Y + vec.Z * vec.Z);
}

AActor* ANonPlayerActorBase::GetTargetActor()
{
	return player;
}

