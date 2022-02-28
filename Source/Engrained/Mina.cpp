// The game and source code belongs to Team 7 Programming team

#include "Mina.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "CrystalProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Shroobs.h"
#include "Health.h"
#include "Pickup.h"
#include "CrystalCluster.h"


// Sets default values
AMina::AMina()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// set our turn rates for input
	BaseTurnRate = 60.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 20.0f;
	
	// Create a camera boom
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 1200.0f; // The camera follows at this distance behind the character	
	SpringArmComp->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 10.f;
	SpringArmComp->bDoCollisionTest = false;
	CameraComp->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnComp"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);

	//Simple melee attack box - this should eventually be put on a socket on the skeleton of the player to follow the attack-animation
	AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerAttack"));
	AttackBox->InitBoxExtent(FVector(30.f, 30.f, 30.f));
	AttackBox->SetupAttachment(RootComponent);
	AttackBox->SetGenerateOverlapEvents(false);
	AttackBox->SetRelativeLocation(AttackPlacement);

	//dashing values
	CanDash = true;
	DashDistance = 4000.f;
	DashCooldown = 1.f;
	DashStop = 0.2f; 
    Jumping = false;
}


// Called when the game starts or when spawned
void AMina::BeginPlay()
{
	Super::BeginPlay();

	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &AMina::OnOverlap);
	CrystalAmmo = 5;

	SpawnPoints.Empty();
}

FHitResult AMina::RayTracer()
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, this);

	/*GetWorld()->LineTraceSingleByObjectType(
		Hit,
		this->GetActorLocation(),
		CameraComp->GetComponentLocation(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel1),
		TraceParams
	);*/	
	GetWorld()->LineTraceSingleByChannel(
		Hit,
		this->GetActorLocation(),
		CameraComp->GetComponentLocation(),
		ECollisionChannel(ECollisionChannel::ECC_GameTraceChannel1),
		TraceParams
	);
	return Hit;
}

void AMina::RayTraceHit()
{
	FHitResult HitResult = RayTracer();

	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit){
		//UE_LOG(LogTemp, Warning, TEXT("%s between player and camera"), *ActorHit->GetName());
		ActorHit->SetActorHiddenInGame(true);
	}
	else {
		//ActorHit->SetActorHiddenInGame(false);
	}
}




// Called every frame
void AMina::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Jumping)
	{
		Jump();
	}

	MinaCurrentLocation = GetActorLocation();
	
	RayTraceHit();

	//FindClosestSpawn();
}

// Called to bind functionality to input
void AMina::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	//Player forward and right movement 
	PlayerInputComponent->BindAxis("Forward", this, &AMina::Forward);
	PlayerInputComponent->BindAxis("Right", this, &AMina::Right);

	PlayerInputComponent->BindAxis("CameraRotate", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("CharacterRot", this, &AMina::TurnAtRate);

	//Actions inputs
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMina::CheckJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMina::CheckJump);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMina::Dash);
	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &AMina::Melee);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AMina::Shoot);
	PlayerInputComponent->BindAction("Melee", IE_Released, this, &AMina::StopMelee);

}

void AMina::CheckJump()
{
	if (Jumping)
	{
		Jumping = false;
	}
	else
	{
		Jumping = true;
	}
}


void AMina::Dash()
{
	if (CanDash)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			DashParticle,
			GetActorLocation(),
		    GetActorRotation(),
			GetActorScale(),
			true,
			true,
			ENCPoolMethod::AutoRelease,
			true
		);
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		LaunchCharacter(FVector(RootComponent->GetForwardVector().X, RootComponent->GetForwardVector().Y, 0).GetSafeNormal() * DashDistance, true, true);
		CanDash = false;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMina::StopDashing, DashStop, false);
	}
}

void AMina::Melee()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack begin"))
	isAttacking = true;
}

void AMina::StopMelee()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack end"))
	isAttacking = false;
}

void AMina::OnAttackBeginOverlap()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack true"))
	AttackBox->SetGenerateOverlapEvents(true);
}

void AMina::OnAttackEndOverlap()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack false"))
	AttackBox->SetGenerateOverlapEvents(false);
}


//This function determines if we overlap the shroob class with our attack hit box
void AMina::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Overlaps %s"), *OtherActor->GetName())
	if (OtherActor->IsA(AShroobs::StaticClass()))
	{
		//We cast to the ImHit function in the shroob class
		Cast<AShroobs>(OtherActor)->ImHit();
		CrystalAmmo++;
	}
	if (OtherActor->IsA(ACrystalCluster::StaticClass()))
	{
		Cast<ACrystalCluster>(OtherActor)->IsStruck();
	}
}

//This function is called whenever the player picks up a ammo refill
void AMina::Refill()
{
	if (CrystalAmmo < 5)
	{
		CrystalAmmo++;
	}
}


void AMina::Shoot()
{
	if (CrystalAmmo > 0)
	{
		if (ProjectileClass)
		{
			FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
			FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

			ACrystalProjectile* TempProjectile = GetWorld()->SpawnActor<ACrystalProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);

			CrystalAmmo--;
		}
	}

}



void AMina::StopDashing()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMina::ResetDash, DashCooldown, false);
}

void AMina::ResetDash()
{
	CanDash = true;

}

void AMina::TurnAtRate(float Rate)
{
	// Calculate delta for the first frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}



void AMina::Forward(float Value)
{
	if (Value != 0.0f)
	{
		// used to find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get the forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMina::Right(float Value)
{

	if (Value != 0.0f)
	{
		// used to find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get the forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}

}


void AMina::InsertSpawnPoint(AActor* Spawn)
{
	for (auto it : SpawnPoints) {
		if (it == Spawn) {
			//UE_LOG(LogTemp, Warning, TEXT("SpawnPoint: %s : has already been pushed"), *Spawn->GetName());
			return;
		}
	}
	SpawnPoints.Push(Spawn);
	//UE_LOG(LogTemp, Warning, TEXT("Pushed Spawn: %s"), *Spawn->GetName());
}

AActor* AMina::FindClosestSpawn()
{
	AActor* ReturnActor = nullptr;
	float Length{1000000}, prev{}, tmp{};
	FVector MinaLocation = GetActorLocation();

	for (auto it : SpawnPoints) {
		//UE_LOG(LogTemp, Display, TEXT("iterator:: %s"), *it->GetName());
		
		FVector SpawnLocation = it->GetActorLocation();
		tmp = LengthBetweenVectors(MinaLocation, SpawnLocation);
		//UE_LOG(LogTemp, Display, TEXT("Length between %s and Mina is %f"), *it->GetName(), tmp);

		if (tmp < Length) {
			Length = tmp;
			ReturnActor = it;
		}
	}

	//if (ReturnActor) {
	//	UE_LOG(LogTemp, Warning, TEXT("Found: %s : as respawnpoint"), *ReturnActor->GetName());
	//}
	//else
	//	UE_LOG(LogTemp, Error, TEXT("No respawn point found"));

	return ReturnActor;
}

void AMina::RespawnPlayer()
{
	AActor* Respawnpoint = FindClosestSpawn();
	if (Respawnpoint) {
		SetActorLocation(Respawnpoint->GetActorLocation() + FVector(0, 0, 200));
	}
}

float AMina::LengthBetweenVectors(FVector vec1, FVector vec2)
{
	FVector v = vec1 - vec2;
	return sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
}
