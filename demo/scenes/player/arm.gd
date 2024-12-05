extends Node3D

@onready var animation = $AnimationPlayer
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if animation.current_animation != ("Pickup"):
		animation.play("Idle")
		
	
	if Input.is_action_just_pressed("pickup"):
		
			animation.play("Pickup")
	#else:
	#	animation.play_backwards("Pickup")	
		
