/*
•	My extensions:
1. Create platforms
2. Add enemies
3. Add sound


•	the bits you found difficult
1. checkContact function for both platform and enemies is difficult to understand, especially the update part for moving enemies.

2. Sometimes the break within the for loop also confuses me because I forget we are only checking one platform, thus once the condition for one specific platform is met, we just need to break out of the loop.

3. Constructor function is also a different concept to create and call. I have to read my notes several times to fully understand how to create and call a constructor function.



•	the skills you learnt/practised by implementing it.

1. I really enjoy searching for sound effects on freesound.org to match with my game project. The skills for adding sounds to p5js is very useful and entertaining.

2. I understand the importance of boolean variables to manipulate whether an action should be implemented.

3. I have a better understanding of this and new keywords.



*/

var gameChar_x;
var gameChar_y;
var floorPos_y;
var scrollPos;
var gameChar_world_x;

var isLeft;
var isRight;
var isFalling;
var isPlummeting;


var clouds;
var mountains;
var trees_x;
var canyons;
var collectables;

var game_score = 0;
var flagpole;
var lives;

var enemies;
var platforms; 

var jumpSound;
var coinSound;
var plummetSound;
var flagpoleSound;
var enemyHarmSound;
var backgroundSound;


function preload()
{
    soundFormats('mp3','wav');
    
    //load your sounds here
    jumpSound = loadSound('assets/jump.wav');
    plummetSound = loadSound('assets/fall.wav');
    flagpoleSound = loadSound('assets/flagpole.wav');
    coinSound = loadSound('assets/coin.wav');
    enemyHarmSound = loadSound('assets/enemyHarm.wav');
    backgroundSound = loadSound('assets/background.wav');
    
    jumpSound.setVolume(0.1);
    plummetSound.setVolume(0.1);
    flagpoleSound.setVolume(0.3);    
    coinSound.setVolume(0.2);   
    enemyHarmSound.setVolume(0.2);  
    backgroundSound.setVolume(0.1);  
}




function setup()
{
	createCanvas(1024, 576);
    floorPos_y = height * 3/4;
    
    // initialise the number of lives
    lives = 3;
    
    backgroundSound.loop();
    
    startGame();  
    
    
}




function startGame()
{
    
	gameChar_x = width/2;
	gameChar_y = floorPos_y;
    
    // Variable to control the background scrolling.
	scrollPos = 0;
    

	// Variable to store the real position of the gameChar in the game
	// world. Needed for collision detection.
	gameChar_world_x = gameChar_x - scrollPos;

    
	// Boolean variables to control the movement of the game character.
	isLeft = false;
	isRight = false;
	isFalling = false;
	isPlummeting = false;
    
    
    
	// Initialise arrays of scenery objects.
    clouds = [
        {x_pos: 100, y_pos: 200},
        {x_pos: 600, y_pos: 100},
        {x_pos: 1100, y_pos: 200},  
        
        {x_pos: 1600, y_pos: 200},
        {x_pos: 2100, y_pos: 100},
        {x_pos: 2600, y_pos: 200},  
    ];
    
    
    
    
    mountains = [
        {x_pos: 200, y_pos: floorPos_y},
        {x_pos: 700, y_pos: floorPos_y},
        {x_pos: 1200, y_pos: floorPos_y},
        
        {x_pos: 1700, y_pos: floorPos_y},
        {x_pos: 2200, y_pos: floorPos_y},
        {x_pos: 2700, y_pos: floorPos_y},
    ];
    
    
    
    
    trees_x = [150, 350, 550, 1050, 1250, 1450, 1950, 2150, 2350];
    



    canyons = [
        {x_pos: 50, width: 100},
        {x_pos: 950, width: 100},
        {x_pos: 1850, width: 100},
    ];
    
    
    
    platforms = [];
    platforms.push(createPlatforms(-50, floorPos_y - 100, 100));
    platforms.push(createPlatforms(900, floorPos_y - 100, 150));
    platforms.push(createPlatforms(1200, floorPos_y - 100, 100));
    platforms.push(createPlatforms(1800, floorPos_y - 100, 100));
    
    
    
    enemies = [];
    enemies.push(new Enemy(250, floorPos_y - 10, 100));
    enemies.push(new Enemy(900, floorPos_y - 10, 100));
    enemies.push(new Enemy(900, floorPos_y - 10, 100));
    enemies.push(new Enemy(1800, floorPos_y - 10, 100));
    
    
    if(lives == 3){
        collectables = [
            {x_pos: -10, y_pos: floorPos_y - 120, size: 50, isFound: false},
            {x_pos: 370, y_pos: floorPos_y - 15, size: 50, isFound: false},
            {x_pos: 1070, y_pos: floorPos_y - 15, size: 50, isFound: false},
            {x_pos: 1270, y_pos: floorPos_y - 120, size: 50, isFound: false},
            {x_pos: 1670, y_pos: floorPos_y - 15, size: 50, isFound: false},
            {x_pos: 1870, y_pos: floorPos_y - 120, size: 50, isFound: false},
        ];    
    }
    

    
    flagpole = {x_pos: 2050,
                isReached: false,
                height: 300}; 
}







function draw()
{    
	background(100, 155, 255); // fill the sky blue

	noStroke();
	fill(0,155,0);
	rect(0, floorPos_y, width, height/4); // draw some green ground
    
    push();
    translate(scrollPos, 0);

	// Draw clouds.
    drawClouds();

    
	// Draw mountains.
    drawMountains();

	// Draw trees.
    drawTrees();
    

	// Draw canyons.
    for(var i = 0; i < canyons.length; i++)
    {
        drawCanyon(canyons[i]);
        checkCanyon(canyons[i]);
    }
    
    
    // Draw the platform
    for(var i = 0; i < platforms.length; i++)
    {
        platforms[i].draw();
    }
    


	// Draw collectable items.
    for(var i = 0; i < collectables.length; i++)
    {
        if(!collectables[i].isFound)
        {
            drawCollectable(collectables[i]);   
            checkCollectable(collectables[i]);        
        }
        
    }
    
    renderFlagpole(flagpole);
    
    
    for(var i = 0; i < enemies.length; i++)
    {
        enemies[i].draw();
        var isContact = enemies[i].checkContact(gameChar_world_x, gameChar_y);
        if(isContact)
        {
            if(lives > 0)
            {
                enemyHarmSound.play();
                lives -= 1;
                startGame();
                break;
            }
            
        }
    }
    
    pop();
    
   
    

	// Draw game character.
	drawGameChar();
   
 
    fill(255);
    noStroke();
    textSize(20);
    text("score: " + game_score, 20, 40);
    text("lives: " + lives, 20, 60);
    
    if(lives < 1)
    {
        textSize(30);
        text("Game over - Refresh Page to continue", width/2 - 200, height/2);
        return;
    }
    
    else if(flagpole.isReached)
    {
        textSize(30);
        text("Level 1 complete - Refresh Page to continue", width/2 - 300, height/2);
        return;
    }
    
    

	// Logic to make the game character move or the background scroll.
	if(isLeft)
	{
		if(gameChar_x > width * 0.2)
		{
			gameChar_x -= 5;
		}
		else
		{
			scrollPos += 5;
		}
	}

    
    
	if(isRight)
	{
		if(gameChar_x < width * 0.8)
		{
			gameChar_x  += 5;
		}
		else
		{
			scrollPos -= 5; // negative for moving against the background
		}
	}

    
    
	// Logic to make the game character rise and fall.  
    if(gameChar_y < floorPos_y)
    {
        var isContact = false;
        
        for(var i = 0; i < platforms.length; i++)
        {
            if(platforms[i].checkContact(gameChar_world_x, gameChar_y))
            {
                isContact = true;
                break;
                
            }
        }
        
        if(!isContact)
        {
            gameChar_y += 2;
            isFalling = true;
        }
        
       
        
    }
    
    else
    {
        isFalling = false;
    
    }
    
    if(isPlummeting)
    {
        gameChar_y += 10;
        plummetSound.play();
       
    }
    
    
    
    // check flagpole
    if(!flagpole.isReached)
    {
        checkFlagpole();
        
    }
    
    checkPlayerDie();
    


	// Update real position of gameChar for collision detection.
	gameChar_world_x = gameChar_x - scrollPos;
}






// ----------------------------------
// checkPlayDie Function
// ----------------------------------
// Function to check the game character whether has fallen below the bottom
function checkPlayerDie()
{
    if(gameChar_y > height)
    {
        if(lives > 0)
        {
            lives -= 1;
            startGame();
        }
    }
}
   


// ---------------------
// Key control functions
// ---------------------
function keyPressed(){

    
    if(key == "A" || keyCode == 37)
    {
        isLeft = true;
    }
    
    else if(key == "D" || keyCode == 39)
    {
        isRight = true;
    }
    
    
    if(key == "W" || keyCode == 32)
    {
        if(!isFalling)
        {
            gameChar_y -= 100;
            jumpSound.play();
        }
    }
}








function keyReleased()
{
    if(key == "A" ||keyCode == 37)
    {
        isLeft = false;
    
    }
    
    else if(key == "D" ||keyCode == 39)
    {
        isRight = false;
    
    }

}








// ------------------------------
// Game character render function
// ------------------------------



// Function to draw the game character.

function drawGameChar()
{
	// draw game character
    if(isLeft && isFalling)
	{
		// add your jumping-left code
        // HEAD
        fill(255, 220, 177);
        ellipse(gameChar_x, gameChar_y - 50, 30, 35);

        // LEFT EYE
        fill(0);
        ellipse(gameChar_x - 8, gameChar_y -50, 4, 6);

        // LEFT EYEBROW
        stroke(0);
        strokeWeight(1);
        line(gameChar_x - 12, gameChar_y - 57, gameChar_x - 6, gameChar_y - 57);


        // HAIR
        fill(103, 204, 235);
        noStroke();
        triangle(gameChar_x + 2, gameChar_y - 70, gameChar_x - 10, gameChar_y - 65, gameChar_x, gameChar_y - 65);


        // MOUTH
        fill(253, 121, 173);
        noStroke();
        triangle(gameChar_x - 10, gameChar_y - 37, gameChar_x - 12, gameChar_y - 42, gameChar_x - 6, gameChar_y - 42);

        // LEFT ARM
        stroke(0);
        line(gameChar_x - 14, gameChar_y - 34, gameChar_x - 22, gameChar_y - 34);


        // BODY
        noStroke();
        fill(255,255,204);
        rect(gameChar_x -13, gameChar_y - 35, 20, 25);


         // LEFT LEG
        fill(255, 204, 153);
        rect(gameChar_x - 13, gameChar_y - 16, 8, 12);

        // RIGHT LEG
        rect(gameChar_x - 20, gameChar_y - 20, 8, 12);
	}
    
    
	else if(isRight && isFalling)
	{
		// add jumping-right code
        // HEAD
        fill(255, 220, 177);
        ellipse(gameChar_x, gameChar_y - 50, 30, 35);

        // RIGHT EYE
        fill(0);
        ellipse(gameChar_x + 8, gameChar_y -50, 4, 6);

        // RIGHT EYEBROW
        stroke(0);
        strokeWeight(1);
        line(gameChar_x + 12, gameChar_y - 57, gameChar_x + 6, gameChar_y - 57);

        // HAIR
        fill(103, 204, 235);
        noStroke();
        triangle(gameChar_x - 5, gameChar_y - 70, gameChar_x + 10, gameChar_y - 65, gameChar_x, gameChar_y - 65);

        // MOUTH
        fill(253, 121, 173);
        noStroke();
        triangle(gameChar_x + 10, gameChar_y - 37, gameChar_x + 12, gameChar_y - 42, gameChar_x + 6, gameChar_y - 42);

        // RIGHT ARM
        stroke(0);
        line(gameChar_x + 8, gameChar_y - 34, gameChar_x + 16, gameChar_y - 34);

        // BODY
        noStroke();
        fill(255,255,204);
        rect(gameChar_x -13, gameChar_y - 35, 20, 25);


        // LEFT LEG
        fill(255, 204, 153);
        rect(gameChar_x + 6, gameChar_y - 20, 8, 12);

        // RIGHT LEG
        rect(gameChar_x , gameChar_y - 14, 8, 12);
	}
    
    
	else if(isLeft)
	{
		// add walking left code
        // HEAD
        fill(255, 220, 177);
        ellipse(gameChar_x, gameChar_y - 50, 30, 35);
    
        // LEFT EYE
        fill(0);
        ellipse(gameChar_x - 8, gameChar_y -50, 4, 6);

        // LEFT EYEBROW
        stroke(0);
        strokeWeight(1);
        line(gameChar_x - 12, gameChar_y - 57, gameChar_x - 6, gameChar_y - 57);

        // HAIR
        fill(103, 204, 235);
        noStroke();
        triangle(gameChar_x + 2, gameChar_y - 70, gameChar_x - 10, gameChar_y - 65, gameChar_x, gameChar_y - 65);

        // MOUTH
        fill(253, 121, 173);
        noStroke();
        triangle(gameChar_x - 10, gameChar_y - 37, gameChar_x - 12, gameChar_y - 42, gameChar_x - 6, gameChar_y - 42);

        // LEFT ARM
        stroke(0);
        line(gameChar_x - 14, gameChar_y - 34, gameChar_x - 22, gameChar_y - 34);

        // RIGHT ARM
        stroke(0);
        line(gameChar_x + 8, gameChar_y - 34, gameChar_x + 16, gameChar_y - 34);

        // BODY
        noStroke();
        fill(255,255,204);
        rect(gameChar_x -13, gameChar_y - 35, 20, 25);
        
        // LEFT LEG
        fill(255, 204, 153);
        rect(gameChar_x - 13, gameChar_y - 12, 8, 12);

        // RIGHT LEG
        rect(gameChar_x  + 5, gameChar_y - 12, 12, 8);
    }


	else if(isRight)
	{
		// add walking right code
        // HEAD
        fill(255, 220, 177);
        ellipse(gameChar_x, gameChar_y - 50, 30, 35);

        // RIGHT EYE
        fill(0);
        ellipse(gameChar_x + 8, gameChar_y -50, 4, 6);

        // RIGHT EYEBROW
        stroke(0);
        strokeWeight(1);
        line(gameChar_x + 12, gameChar_y - 57, gameChar_x + 6, gameChar_y - 57);

        // HAIR
        fill(103, 204, 235);
        noStroke();
        triangle(gameChar_x - 5, gameChar_y - 70, gameChar_x + 10, gameChar_y - 65, gameChar_x, gameChar_y - 65);

        // MOUTH
        fill(253, 121, 173);
        noStroke();
        triangle(gameChar_x + 10, gameChar_y - 37, gameChar_x + 12, gameChar_y - 42, gameChar_x + 6, gameChar_y - 42);

        // LEFT ARM
        stroke(0);
        line(gameChar_x - 14, gameChar_y - 34, gameChar_x - 22, gameChar_y - 34);


        // RIGHT ARM
        stroke(0);
        line(gameChar_x + 8, gameChar_y - 34, gameChar_x + 16, gameChar_y - 34);

        // BODY
        noStroke();
        fill(255,255,204);
        rect(gameChar_x -13, gameChar_y - 35, 20, 25);


        // LEFT LEG
        fill(255, 204, 153);
        rect(gameChar_x - 22, gameChar_y - 12, 12, 8);

        // RIGHT LEG
        rect(gameChar_x , gameChar_y - 12, 8, 12);
	}
    
    
	else if(isFalling || isPlummeting)
	{
		// add jumping facing forwards code
        // HEAD
        fill(255, 220, 177);
        ellipse(gameChar_x, gameChar_y - 50, 35, 35);

        // LEFT EYE
        fill(0);
        ellipse(gameChar_x - 5, gameChar_y -50, 5, 7);

        // LEFT EYEBROW
        stroke(0);
        strokeWeight(1);
        line(gameChar_x - 9, gameChar_y - 57, gameChar_x - 3, gameChar_y - 57);

        // RIGHT EYE
        noStroke();
        fill(0);
        ellipse(gameChar_x + 8, gameChar_y -50, 5, 7);

         // RIGHT EYEBROW
        stroke(0);
        line(gameChar_x + 5, gameChar_y - 57, gameChar_x + 11, gameChar_y - 57);

        // HAIR
        fill(103, 204, 235);
        noStroke();
        triangle(gameChar_x - 5, gameChar_y - 70, gameChar_x + 10, gameChar_y - 65, gameChar_x, gameChar_y - 65);

        // MOUTH
        fill(253, 121, 173);
        noStroke();
        triangle(gameChar_x + 1, gameChar_y - 37, gameChar_x - 4, gameChar_y - 42, gameChar_x + 6, gameChar_y - 42);

        // LEFT ARM
        stroke(0);
        line(gameChar_x - 14, gameChar_y - 34, gameChar_x - 23, gameChar_y - 30);

        // RIGHT ARM
        stroke(0);
        line(gameChar_x + 14, gameChar_y - 34, gameChar_x + 23, gameChar_y - 34);

        // BODY
        noStroke();
        fill(255,255,204);
        rect(gameChar_x -13, gameChar_y - 35, 26, 25);

       // LEFT LEG
        fill(255, 204, 153);
        rect(gameChar_x - 13, gameChar_y - 14, 8, 12);

        // RIGHT LEG
        rect(gameChar_x  + 5, gameChar_y - 18, 8, 12);      
	}
    
    
	else
	{
		// add standing front facing code
        // HEAD
        fill(255, 220, 177);
        ellipse(gameChar_x, gameChar_y - 50, 35, 35);

        // LEFT EYE
        fill(0);
        ellipse(gameChar_x - 5, gameChar_y -50, 5, 7);

        // LEFT EYEBROW
        stroke(0);
        strokeWeight(1);
        line(gameChar_x - 9, gameChar_y - 57, gameChar_x - 3, gameChar_y - 57);

        // RIGHT EYE
        noStroke();
        fill(0);
        ellipse(gameChar_x + 8, gameChar_y -50, 5, 7);

         // RIGHT EYEBROW
        stroke(0);
        line(gameChar_x + 5, gameChar_y - 57, gameChar_x + 11, gameChar_y - 57);

        // HAIR
        fill(103, 204, 235);
        noStroke();
        triangle(gameChar_x - 5, gameChar_y - 70, gameChar_x + 10, gameChar_y - 65, gameChar_x, gameChar_y - 65);

        // MOUTH
        fill(253, 121, 173);
        noStroke();
        triangle(gameChar_x + 1, gameChar_y - 37, gameChar_x - 4, gameChar_y - 42, gameChar_x + 6, gameChar_y - 42);

        // LEFT ARM
        stroke(0);
        line(gameChar_x - 14, gameChar_y - 34, gameChar_x - 21, gameChar_y - 20);

        // RIGHT ARM
        stroke(0);
        line(gameChar_x + 14, gameChar_y - 34, gameChar_x + 21, gameChar_y - 20);

        // BODY
        noStroke();
        fill(255,255,204);
        rect(gameChar_x -13, gameChar_y - 35, 26, 25);

        // LEFT LEG
        fill(255, 204, 153);
        rect(gameChar_x - 13, gameChar_y - 12, 8, 12);

        // RIGHT LEG
        rect(gameChar_x  + 5, gameChar_y - 12, 8, 12);
	}
    
}



// ---------------------------
// Background render functions
// ---------------------------

// Function to draw cloud objects.
function drawClouds()
{
    for(var i = 0; i < clouds.length; i++)
    {
        fill(255);
        ellipse(clouds[i].x_pos, clouds[i].y_pos, 70, 80);
        ellipse(clouds[i].x_pos - 30, clouds[i].y_pos,70, 60);
        ellipse(clouds[i].x_pos + 30, clouds[i].y_pos, 70, 60);
    }
    
}




// Function to draw mountains objects.
function drawMountains()
{
    for(var i = 0; i < mountains.length; i++)
    {
        fill(235,151,42);
        triangle(mountains[i].x_pos, mountains[i].y_pos,  
                 mountains[i].x_pos + 200, mountains[i].y_pos, 
                 mountains[i].x_pos + 100,  mountains[i].y_pos - 332);
        
        fill(183, 121, 50);
        triangle(mountains[i].x_pos + 150, mountains[i].y_pos, 
                 mountains[i].x_pos + 290, mountains[i].y_pos, 
                 mountains[i].x_pos + 220, mountains[i].y_pos - 232);
        
        fill(176, 107, 16);
        triangle(mountains[i].x_pos + 100, mountains[i].y_pos, 
                 mountains[i].x_pos + 250, mountains[i].y_pos, 
                 mountains[i].x_pos + 175, mountains[i].y_pos- 157);
    }  
}




// Function to draw trees objects.
function drawTrees()
{
    for(var i = 0; i < trees_x.length; i++)
    {
        // tree trunk
        fill(120, 100, 40);
        rect(trees_x[i], floorPos_y - 144, 60, 144);
        
        // branches of the tree
        fill(0, 155, 0);
        triangle(trees_x[i] - 50, floorPos_y - 94, 
                 trees_x[i] + 30, floorPos_y - 194, 
                 trees_x[i] + 110, floorPos_y - 94);
        triangle(trees_x[i] - 50, floorPos_y - 144, 
                 trees_x[i] + 30, floorPos_y - 244, 
                 trees_x[i] + 110, floorPos_y - 144);   
    } 
}






// ---------------------------------
// Canyon render and check functions
// ---------------------------------

// Function to draw canyon objects.
function drawCanyon(t_canyon)
{
    fill(240, 176, 120);
    rect(t_canyon.x_pos, floorPos_y, t_canyon.width, height - floorPos_y);

    // canyon's entrance left shape 
    fill(117, 97, 43);
    beginShape();
    vertex(t_canyon.x_pos, t_canyon.width + 332);
    vertex(t_canyon.x_pos + 10, t_canyon.width + 338);
    vertex(t_canyon.x_pos + 10, t_canyon.width+ 476);
    vertex(t_canyon.x_pos, t_canyon.width+ 476);
    vertex(t_canyon.x_pos, t_canyon.width + 332);
    endShape();

    // canyon's entrance right shape 
    fill(117, 97, 43);
    beginShape();
    vertex(t_canyon.x_pos + 100, t_canyon.width + 332);
    vertex(t_canyon.x_pos + 90, t_canyon.width + 338);
    vertex(t_canyon.x_pos + 90, t_canyon.width + 476);
    vertex(t_canyon.x_pos + 100, t_canyon.width + 476);
    vertex(t_canyon.x_pos + 100, t_canyon.width + 332);
    endShape();

}

// Function to check character is over a canyon.
function checkCanyon(t_canyon)
{
    if(gameChar_world_x > t_canyon.x_pos  && 
       gameChar_world_x < t_canyon.x_pos + t_canyon.width &&
       gameChar_y >= floorPos_y)
    {
        isPlummeting = true;
    }
}



// ----------------------------------
// Collectable items render and check functions
// ----------------------------------

// Function to draw collectable objects.
function drawCollectable(t_collectable)
{
    fill(212, 175, 55);
    ellipse(t_collectable.x_pos, t_collectable.y_pos, t_collectable.size - 20, t_collectable.size - 20);
    fill(255, 223, 0, 100);
    ellipse(t_collectable.x_pos, t_collectable.y_pos, t_collectable.size - 30, t_collectable.size - 30);

   // dollars sign
    fill(212, 175, 55);
    textStyle(BOLD);
    textAlign(CENTER);
    text("$", t_collectable.x_pos, t_collectable.y_pos + 5);
}


// Function to check character has collected an item.
function checkCollectable(t_collectable)
{
    if(dist(gameChar_world_x, gameChar_y, t_collectable.x_pos, t_collectable.y_pos) < t_collectable.size)
    {
        t_collectable.isFound = true;
        coinSound.play();
        game_score += 1;
    }
}



// ----------------------------------
// Flagpole render and check functions
// ----------------------------------

// Function to render flagpole

function renderFlagpole(t_flagpole)
{
    push();
    strokeWeight(5);
    stroke(180);
    line(t_flagpole.x_pos, floorPos_y, t_flagpole.x_pos, floorPos_y - 250);
    
    fill(255, 0, 0);
   
    noStroke();
   
    if(t_flagpole.isReached)
    {
        rect(t_flagpole.x_pos, floorPos_y - 250, 50, 50);
    }
    else
    {
        rect(t_flagpole.x_pos, floorPos_y - 50, 50, 50);
        
    }
    
    pop();
    
    
}


// Function to check Flagpole
function checkFlagpole()
{
    var d = dist(gameChar_world_x, 0, flagpole.x_pos, 0);
    if(d < 15)
    {
        flagpole.isReached = true;
        flagpoleSound.play();
    }
}



// the factory pattern for creating platforms
function createPlatforms(x, y, length)
{
    var p = 
    {
        x: x,
        y: y,
        length: length,
        draw: function(){
            fill(255, 0, 255);
            rect(this.x, this.y, this.length, 20);
        },
        checkContact: function(gc_x, gc_y)
        {
            if(gc_x > this.x && gc_x < this.x + this.length)
            {
                var d = this.y - gc_y
                if(d >= 0 && d < 5)
                {
                    return true;
                }
        
            }
            
            return false;    
        }  
    }
    
    return p;
}




// constructor function Enemy
function Enemy(x, y, range)
{
    this.x = x;
    this.y = y;
    this.range = range;
    
    this.currentX = x;
    this.inc = 1;
    
    this.update = function()
    {
        this.currentX += this.inc;
        
        if(this.currentX >= this.x + this.range)
        {
            this.inc = -1;
        }
        
        else if(this.currentX < this.x)
        {
            this.inc = 1;
        }
        
        
    }
    
    this.draw = function()
    {
        this.update();
        fill(255, 0, 0);
        ellipse(this.currentX, this.y - 10, 30, 30);
        fill(255, 255, 255);
        ellipse(this.currentX, this.y - 10, 10, 10);
        
    }
        
    
    this.checkContact = function(gc_x, gc_y)
    {
        var d = dist(gc_x, gc_y, this.currentX, this.y)
        if(d < 15)
        {
            return true;    
        }
        
        else
        {
            return false;
        }
        
    }   
}