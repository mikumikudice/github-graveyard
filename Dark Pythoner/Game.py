import random as rand

#System
System_R = {"Version" : "4.1.3.4", "EXP" : 0, "GameOver" : False}

#Character
CharString = {"Name" : "Empty", "Breed" : "Empty", "Class" : "Empty"}
CharNumber = {"Life" : "Empty", "Level" : "Empty", "Money" : "Empty"}
Inventory = {"LftHd" : "Empty", "RgtHd" : "Empty", "Potion" : "Empty"}

#Items
SwordItems = ["Magic Sword", "Nexus Sword", "Dervonium Sword", "Diamond Sword", "Terdromium mega Sword"]
BowedItems = ["Gargoto Bow", "Sworded Bow", "Omegan Crossbow", "Lelbb'one Bow", "Neutroviuned Crossbow"]
StaffItems = ["Venus Staff", "Night Staff", "Infernite Staff", "Gerremy Staff", "Taumalated beta Staff"]
HeavyItems = ["Axed Hammer", "Hagurko Axe", "Betanull Hammer", "Morttalor Axe", "Head-Destroyer Hammer"]

DrinkItems = ["Life Potion", "Speed Potion", "Hyper Potion", "? Potion"]
RHandItems = ["Bandages", "Poison Bottle", "Fired Bottle", "Ice Bottle"]

#Items Val
DataBank = {"Short Sword" : 2, "Magic Sword" : 3, "Nexus Sword" : 4, "Dervonium Sword" : 9, "Diamond Sword" : 12, "Terdromium mega Sword" : 16,
            "Simple Bow"  : 2, "Gargoto Bow" : 8, "Sworded Bow" : 5, "Omegan Crossbow" : 7, "Lelbb'one Bow" : 10, "Neutroviuned Crossbow" : 24,
            "Basic Staff" : 3, "Venus Staff" : 6, "Night Staff" : 5, "Infernite Staff" : 8, "Gerremy Staff" : 11, "Taumalated beta Staff" : 13,
            "Old Hammer"  : 6, "Axed Hammer" : 9, "Hagurko Axe" : 6, "Betanull Hammer" : 8, "Morttalor Axe" : 13, "Head-Destroyer Hammer" : 14,
            
            "Life Potion": 4, "Speed Potion" : 7, "Hyper Potion" : 9, "? Potion"   : 1,
            "Bandages"   : "-", "Poison Bottle" : 4, "Fired Bottle": 7, "Ice Bottle" : 3,

            "Empty" : 0}

#Monsters
EnemyList = ["Goblins", "Little Knights", "Ghosts", "Lizards", "a Archer", "a Troll", "a Dark Witch", "a Orc", "a Hyper Warrior", "a Dragon"]
Boss_List = ["The Dragon Quin", "a Big Demon", "a Giant Spider", "The fucking Death"]

#_SystemCall_#--------------------------------------------------------------------------------------------#

#First Call
def Start(): #

    print("Version : "+System_R["Version"]+"\n")
    print("Old saves (Version 4.0.4.6-) don't work now (Version 4.0.4.6+)\n")
    FileMananger = Question("To load the game Type", "0", "To play New Game Type", "1", 2)

    if FileMananger == True : LoadGame()
    if FileMananger == False : NewChar()
    if FileMananger == "Idiot" : Start()
#

#Get data in "Save.txt"
def LoadGame() : #

    #Open file
    File = open("Save.txt", "r")

    #Numbers
    CharNumber["Life"] = int(File.readline().replace('\n', ''))
    CharNumber["Level"] = int(File.readline().replace('\n', ''))
    CharNumber["Money"] = int(File.readline().replace('\n', ''))

    #String vaules
    CharString["Name"] = str(File.readline().replace('\n', ''))
    CharString["Breed"] = str(File.readline().replace('\n', ''))
    CharString["Class"] = str(File.readline().replace('\n', ''))

    #Items
    Inventory["LftHd"] = str(File.readline().replace('\n', ''))
    Inventory["RgtHd"] = str(File.readline().replace('\n', ''))
    Inventory["Potion"] = str(File.readline().replace('\n', ''))

    File.close()
    Hall()
#

#Write in "Save.txt"
def SaveGame() : #

    File = open("Save.txt", "w")
        
    File.write(
        str(CharNumber["Life"])+"\n"+
        str(CharNumber["Level"])+"\n"+
        str(CharNumber["Money"])+"\n"+
        CharString["Name"]+"\n"+
        CharString["Breed"]+"\n"+
        CharString["Class"]+"\n"+
        str(Inventory["LftHd"])+"\n"+
        str(Inventory["RgtHd"])+"\n"+
        str(Inventory["Potion"])+"\n")
        
    File.close()
    
    print("\nGame saved!")
    Hall()
#

#Add XP to EXP and make "Level Up"
def EXP_Up(XP) : #

    #AntiBug
    if System_R["GameOver"] == True : return 0
    
    print("\nYou got "+str(XP)+" xp points!")
    System_R["EXP"] += XP
    print("EXP : "+str(System_R["EXP"]))

    if System_R["EXP"] >= 20 and CharNumber["Level"] == 1 :

        print("\nLevel up!\nLevel 2")
        CharNumber["Life"] += XP
        CharNumber["Level"] = 2

    if System_R["EXP"] >= 40 and CharNumber["Level"] == 2 :

        print("\nLevel up!\nLevel 3")
        CharNumber["Life"] += XP * 2
        CharNumber["Level"] = 3

    if System_R["EXP"] >= 80 and CharNumber["Level"] == 3 :

        print("\nLevel up!\nLevel 4")
        CharNumber["Life"] += XP * 3
        CharNumber["Level"] = 4

    if System_R["EXP"] >= 160 and CharNumber["Level"] == 4 :

        print("\nOver Power!\nLevel 5")
        CharNumber["Life"] += XP * 4
        CharNumber["Level"] = 5

    return 0
#

def Exit() : #

    print("Bye!")
    input()
    io.exit()
#

#__GameRoom__#--------------------------------------------------------------------------------------------#

#The default "scene" of game
def Hall() : #

    if System_R["GameOver"] == True : return 0

    print("\nWellcome back, "+CharString["Name"]+"!")
    print("\n See Stats : Type 1"+
          "\n Travel to : Type 2"+
          "\n Save game : Type 3"+
          "\n Exit game : type 4")
    
    IWant = input("\nChoice : ")

    #See myself
    if IWant == "1" :
        
        print("\nYou are in level "+
              str(CharNumber["Level"])+
              " and with "+
              str(System_R["EXP"])+" of XP"+
              
              "\n\n Life points : "+
              str(CharNumber["Life"])+
              
              "\n Your money  : "
              +str(CharNumber["Money"])+"\n"+
              
              "\n Lft Hand : "+Inventory["LftHd"]+"("
              +str(DataBank[Inventory["LftHd"]])+")"+
              
              "\n Rgt Hand : "+Inventory["RgtHd"]+"("
              +str(DataBank[Inventory["RgtHd"]])+")"+
              
              "\n Potions  : "+Inventory["Potion"]+
              "\n")
        
        CmBk = Question("Come back : Type", "0", "", "", 1)

        if CmBk == True or CmBk == False: Hall();

    #Travel to
    if IWant == "2" :

        print("\nWhere are you going?\n"+
              "\n Dungeons : Type 1"+
              "\n Shopping : Type 2"+
              "\n Quests   : Type 3"+
              "\n Cancel   : Type 4"+
              "\n")

        GngT = input("Going to : ")

        if GngT == "1" :

            print("\nGood luck!")
            Dungeon(1, CharNumber["Life"])
            
        if GngT == "2" : GngT = Shopping(1)
            
        if GngT == "3" : Quest()
            
        if GngT == "4" : Hall()

    if IWant == "3" : SaveGame()
    if IWant == "4" : Exit()
#

def Dungeon(Room, Life) : #

    #AntiBug
    if System_R["GameOver"] == True : return 0
    
    print("\nCurrent life : "+str(Life))

    #1st Monster room
    if Room == 1 : NewEnemy("1st room", Room, "Enemy")

    #Shopping Room
    if Room == 2 : Room = Shopping(2)

    #2st Monster Room
    if Room == 3 : NewEnemy("3st room", Room, "Enemy")

    #Chest room
    if Room == 4 : Room = Chest()

    #Boss room
    if Room == 5 : NewEnemy("Last room", Room, "Boss")

    if Room == 6 :
            
        Says = ["\"We hope see you again!\"", "\"Next time we'll catch you!\"", "Hum... So, do you win?\""]

        MMessage = rand.choice(Says)+" - Monsters "+str(rand.randrange(1, 30))+"/"+str(rand.randrange(1, 12))+"/"+str(rand.randrange(1100, 1500))
        
        print("\nCongratulations! You survived the dungeon!")
        print("-" * len(MMessage))
        print(MMessage)

        Hall()
#

#The shopping room
def Shopping(WhoCalls) : #

    #AntiBug
    if System_R["GameOver"] == True : return 0 

    #Hello
    if WhoCalls == 1 : print("\nWellcome to the Shopping!")
    if WhoCalls == 2 : print("\nShopping room")

    Item = ""

    #Define item type
    if CharString["Class"] == "Knight" : Item = rand.choice(SwordItems)
    if CharString["Class"] == "Archer" : Item = rand.choice(BowedItems)
    if CharString["Class"] == "Wizard" : Item = rand.choice(StaffItems)
    if CharString["Class"] == "Brewer" : Item = rand.choice(HeavyItems)

    #Price
    Price = DataBank[Item] + rand.randrange(1, 30)
    
    #Exceptions
    if Item == Inventory["LftHd"] :

        print("The shop are empty today...")
        if WhoCalls == 1 : Hall()
        if WhoCalls == 2 : return 3

    else :

        YourMoney = CharNumber["Money"]
    
        print("\nYour money : "+str(YourMoney))
        print("\nToday I have a "+Item+" to you, for "+str(Price)+"\n")

        YoN = Question("Buy item : Type", "0", "Refuse : Type", "1", 2)

        if YoN == True :

            if Price <= YourMoney :

                Inventory["LftHd"] = Item
                print("\nAdded "+Item+" to inventory")
                CharNumber["Money"] -= Price
                
                if WhoCalls == 1 : Hall()
                if WhoCalls == 2 : return 3

            else :

                print("You have no money enough!")
                if WhoCalls == 1 : Hall()
                if WhoCalls == 2 : return 3

        if YoN == False :

            print("\nPlease come back again!")
            if WhoCalls == 1 : Hall()
            if WhoCalls == 2 : return 3
# 

#Run quests
def Quest() : #

    if System_R["GameOver"] == True : return 0

    #Variables
    if CharNumber["Money"] < 100 : Time = 100 - CharNumber["Money"]
        
    Wait = rand.randrange(1, Time)
    Percent = 1
    i = 0

    Quests = ["kill a dude", "save my friend", "buy a goat"] 
    Pronoms = ["me", "my wife", "the kingdom", "my village", "my people", "my dark cat"]

    TheQuest = ("\nCan you "+rand.choice(Quests)+" for "+rand.choice(Pronoms)+"?\n")

    CmBck = ""

    while i < Wait :
    
        i += 0.1
        Percent = (i / Wait) * 100

        print(TheQuest, str(round(Percent))+"% Completed")
            
        if i >= Wait :
            
            print("\nYou got it! And you gained "+str(Wait)+" pieces of Gold")
            CharNumber["Money"] += Wait
            EndQuest = Question("To come back type", "0", "", "", 1)

    if EndQuest == True :

        print("\nOk")
        Hall()
        
    if CharNumber["Money"] == 100 :

        print("You don´t have quests today!")
        Hall()
#

#_BasicCalls_#--------------------------------------------------------------------------------------------#

#Function to make a question
def Question(FQst, FR, SQst, SR, NA) : #

    if NA == 2 :

        Input = str(input(FQst+" "+FR+"\n"+SQst+" "+SR+"\n\nChoice : "))
        Running = False
            
    if NA == 1 :

        Input = str(input(FQst+" "+FR+"\n\nChoice : "))
        Running = False

    if Input == FR : return True
    if Input == SR : return False
    if Input != FR and Input != SR :
        print("\nI couldn't fix that, so please restart\nerror{245}")
#

#Create a new enemy and calls Battle function
def NewEnemy(RoomN, Room, Type) : #

    print("\n"+RoomN)

    #Type of monster
    if Type == "Enemy" : Enemy = rand.choice(EnemyList)
    if Type == "Boss"  : Enemy = rand.choice(Boss_List)
            
    print("You found "+Enemy+"!")
    Battle(CharNumber["Life"], Enemy, [], True, Room)
#

#Returns the bonus of every item
def Item() : #

    item = Inventory["RgtHd"]

    if item.endswith("Shield") :
            
        print("You have a bonus of "+str(DataBank[item])+" shield points")
        return DataBank[item]
            
    if item.endswith("Book") :

        print("You have a bonus of "+str(DataBank[item])+" knowege points")
        return DataBank[item]

    if item.endswith("Bottle") :

        print("You have a bonus of "+str(DataBank[item])+" efect points")
        return DataBank[item]

    if item == "Bandages" :

        print("Your life is full again!")
        return 999

    if item == "Blank" :

        print("You have no a item")
        return 0
#

def Potion() : #

    Item = Inventory["Potion"]

    print("\nIn Inventory : "+Item)

    if Item.startswith("Life") :

        print("\nUsed "+Item+"!")
        return "Life"
        
    if Item.startswith("Speed") :

        print("\nUsed "+Item+"!")
        return "Speed"
        
    if Item.startswith("Shield") :

        print("\nUsed "+Item+"!")
        return "Shield"

    if Item.startswith("Hyper") :

        print("\nUsed "+Item+"!")
        return 999
    
    if Item.startswith("?") :

        Efect = rand.choice(["Life", "Speed", "Shield"])
        return Efect

    if Item == "Empty" :
        
        print("\nYou have no a potion")
        return "None"
#

#Gives a random item
def Chest() : #

    #AntiBug
    if System_R["GameOver"] == True : return 0
    
    print("\nYou found a chest!")

    Item = ""
    Luck = rand.choice(["Item", "Potion"])
    
    if Luck == "Item" : Item = rand.choice(RHandItems)
    if Luck == "Potion" : Item = rand.choice(DrinkItems)
    
    print("\nAnd inside a "+Item+"!\n")

    chest = Question("To catch the chest item type", "0", "To leave the item type", "1", 2)

    if chest == True :
            
        if Luck == "Item" : Inventory["RgtHd"] = Item
        if Luck == "Potion" : Inventory["Potion"] = Item
        
        print("Added "+Item+" to inventory")
        return 5

    if chest == False :

        print("\nOk, next room!")
        return 5
#

def Battle(Life, Enemy, EStatus, FstCll, Room) : #

    Fighting = False
    HeroTurn = True
    PotionType = ""
    Bonus = 0

    if FstCll == True : EStatus = Bestiary(Enemy)

    if Fighting == False and Life != None :

        HeroTurn = rand.choice([True, False])

        if HeroTurn == True : print("\nNext turn you attack first")
        if HeroTurn == False : print("\nNext turn enemy attacks firt")
        
        print("\nWhat you do?\n"+
              "\n Attack : Type 1"+
              "\n Potion : Type 2"+
              "\n Item   : Type 3"+
              "\n")
        
        IDo = input("I do : ")
  
        if IDo == "1" : Fighting = True
            
        if IDo == "2" : 

            PotionType = Potion()
            
            if PotionType == "Life" : Life = CharNumber["Life"]
            if PotionType == "Speed" : HeroTurn = True
            if PotionType == "Shield" : Life += DataBank[Inventory["Potion"]]
            if PotionType == "None" : print("\n Good Luck...")
            
            if PotionType == 999 : Life = CharNumber["Life"]

            Inventory["Potion"] = "Empty"

            if PotionType == 999 : HeroTurn = True
            else : HeroTurn = False
            
            Fighting = True
            
        if IDo == "3" :

            if Inventory["RgtHd"] == "Empty" : print("\nYou don't have a item")
            
            else :

                Bonus = Item()

                #Bandages
                if Bonus == 999 :

                    Life = CharNumber["Life"]
                    Bonus = 0
                    Inventory["RgtHd"] == "Empty"

                #Bottles
                if Inventory["RgtHd"].endswith("Bottle") :

                    Inventory["RgtHd"] == "Empty"
            
            HeroTurn = False
            Fighting = True

    #On Battle
    if Fighting == True :

        #Hero attacks first
        if HeroTurn == True :

            print("\nYour Turn")
    
            EStatus = HeroAttack(Bonus, Enemy, EStatus)
            Bonus = 0

            if EStatus["Life"] > 0 :
                
                print("\nEnemy turn")

                Life = EnemyAttack(Life, Bonus, Enemy, EStatus)

        #Enemy attacks first
        if HeroTurn == False :

            print("\nEnemy turn")

            Life = EnemyAttack(Life, Bonus, Enemy, EStatus)

            try :
                
                if Life > 0 :
                
                    print("\nYour Turn")
    
                    EStatus = HeroAttack(Bonus, Enemy, EStatus)
                    Bonus = 0

            except TypeError : return 0

        #So?
        if EStatus["Life"] > 0 : Battle(Life, Enemy, EStatus, False, Room)
        if EStatus["Life"] <= 0 : Dungeon(Room + 1, Life)
#

#Make the player actions in battle
def HeroAttack(Bonus, EnemyN, EnStts) : #

    #AntiBug
    if System_R["GameOver"] == True : return 0

    #Hit
    Hit = (rand.randrange(0, CharNumber["Level"]) + 1) #Random hit
    hit = Hit + DataBank[Inventory["LftHd"]] + Bonus   #Weapon Bonus 
    print("\nYou attacked "+EnemyN+" and he loses "+str(hit)+" life points!")

    #Lose life
    if EnStts["NumberOf"] > 1 :

        EnStts["NumberOf"] -= 1

        #Message
        print("\nYou killed one, but have "+str(EnStts["NumberOf"])+" enemmies")
        return EnStts
        
    if EnStts["NumberOf"] == 1 :

        EnStts["Life"] -= hit

        #Messages
        if EnStts["Life"] > 0 :

            print("He is live yet! ("+str(EnStts["Life"])+") Life Points")
            return EnStts
            
        if EnStts["Life"] <= 0 :
            
            print("You killed him!")
            EXP_Up(EnStts["Force"])
            return EnStts
#

#Make the enemy actions in battle
def EnemyAttack(PLife, Bonus, EnemyN, Status) : #

    #AntiBug
    if System_R["GameOver"] == True : return 0

    #Hit
    Hit = (rand.randrange(0, Status["Force"]) + 1)

    #Total hit
    if Bonus < Hit : Hit -= Bonus
    else : Hit = 0
    
    print("\nYou were attacked by "+EnemyN+" and lost "+str(Hit)+"(-"+str(Bonus)+") life points!")

    PLife -= Hit

    if PLife > 0 :
            
            print("You are live yet, "+str(PLife)+" life points")
            return PLife
            
    if PLife <= 0 :

        print("\nYou died!")
        Exit()
#

#Returns the enemy status
def Bestiary(Enemy) : #

    #AntiBug
    if System_R["GameOver"] == True : return 0
            
    #Status
    NumberOf = 1
    MLife = 1
    Force = 1

    #Low Level Enemies
    if Enemy == "Goblins" or Enemy == "Little Knights" :

        NumberOf = 3
        MLife = 4 * CharNumber["Level"]
        Force = 3 * CharNumber["Level"]

    if Enemy == "Ghosts" or Enemy == "Lizards" :

        NumberOf = 2
        MLife = 3 * CharNumber["Level"]
        Force = 2 * CharNumber["Level"]

    #Medium Level Enemies
    if Enemy == "a Archer" or Enemy == "a Dark Witch" :

        NumberOf = 1
        MLife = 4 * CharNumber["Level"]
        Force = 2 * CharNumber["Level"]

    if Enemy == "a Troll" or Enemy == "a Orc" :

        NumberOf = 1
        MLife = 5 * CharNumber["Level"]
        Force = 4 * CharNumber["Level"]

    #Hight Level Enemies
    if Enemy == "a Hyper Warrior" or Enemy == "a Dragon" :

        NumberOf = 1
        MLife = 6 * CharNumber["Level"]
        Force = 3 * CharNumber["Level"]

    #Bosses
    if Enemy == "The Dragon Quin" or Enemy == "a Giant Spider":

        NumberOf = 1
        MLife = 12 * CharNumber["Level"]
        Force = 4 * CharNumber["Level"]

    if Enemy == "The fucking Death" or Enemy == "a Big Demon" :

        NumberOf = 1
        MLife = 15 * CharNumber["Level"]
        Force = 8 * CharNumber["Level"]

    List = {"NumberOf" : NumberOf, "Life" : MLife, "Force" : Force}

    print("\n"+Enemy+" : "+str(List))
    return List
#

#_MakingChar_#--------------------------------------------------------------------------------------------#

def NewChar() : #

    #Name
    print("\nHello!")
    Input = input("What is your name : ")
    SureA = Question("\n"+Input+", right?\n\nYes : Type", "0", "Not : Type", "1", 2)

    #Are you sure?
    if SureA == True :

        CharString["Name"] = Input
        MakeChar()
        
    if SureA == False :

        print("Ok...")
        NewChar()
#

def MakeChar() : #

    #Satatus
    CharNumber["Life"] = rand.randrange(1, 8)
    CharNumber["Level"] = 1
    CharNumber["Money"] = rand.randrange(1, 100)

    #Breed
    print("\nNow pick your breed\n\n Gnome : Type 1\n Ghost : Type 2\n Troll : Type 3\n Demon : Type 4\n Elfic : Type 5")
    Breed = input("\nBreed : ")

    #Gnome
    if Breed == "1" : CharString["Breed"] = "Gnome"
    
    #Ghost
    if Breed == "2" : CharString["Breed"] = "Ghost"

    #Troll
    if Breed == "3" : CharString["Breed"] = "Troll"

    #Demon
    if Breed == "4" : CharString["Breed"] = "Demon"

    #Elfic
    if Breed == "5" : CharString["Breed"] = "Elfic"

    print("\nNow pick a class :\n\n Knight : Type 1\n Archer : Type 2\n Wizard : Type 3\n Brewer : Type 4")
    Class = input("\nClass : ")

    #Knight
    if Class == "1" : CharString["Class"] = "Knight"

    #Archer
    if Class == "2" : CharString["Class"] = "Archer"

    #Wizard
    if Class == "3" : CharString["Class"] = "Wizard"

    #Brewer
    if Class == "4" : CharString["Class"] = "Brewer"

    #Weapons
    if CharString["Class"] == "Knight" : Inventory["LftHd"] = "Short Sword"
    if CharString["Class"] == "Archer" : Inventory["LftHd"] = "Simple Bow"
    if CharString["Class"] == "Wizard" : Inventory["LftHd"] = "Basic Staff"
    if CharString["Class"] == "Brewer" : Inventory["LftHd"] = "Old Hammer"
    
    print("\n Name  : "+CharString["Name"]+
          "\n Breed : "+CharString["Breed"]+
          "\n Class : "+CharString["Class"]+"\n"+
          "\n Life  : "+str(CharNumber["Life"])+
          "\n Money : "+str(CharNumber["Money"])+
          "\n Weapon: "+Inventory["LftHd"]+
          "\n")

    Ok = Question("Ok?\n\nYes : Type", "0", "Not : Type", "1", 2)

    if Ok == True : SaveGame()
    if Ok == False : MakeChar()
#

Start()
