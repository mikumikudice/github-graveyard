-- Own functions  --
require("SystemLib")

local data = {}

data._name = ''
data._capt = 00
data._city = ''

-- His items --
data._invt = 'flashlight;'

-- Alignment --
data._self = '0'

function main()

    while true do
        
        if data._capt == 00 then capt_00() end
        if data._capt == 01 then capt_01() end
        if data._capt == 02 then capt_02() end
    end
end

function capt_00()
	
    echo("Chapter %")
    print("I\n")
    wait(0.5)

    echo("You, alone and with nowhere to go, come to a small town, walled by a low brick wall.")
    echo("\nIn the sky, there is no sunlight, just a deep gray, pale clouds, and an endless drizzle.\n")
    echo("\nAt the entrance, in front of you, is an old gentleman, downcast and\t bluish.\nHe looks to you.\n")
    
    echo("\nGentleman: Well come to this rainy city.\nWhat is your name, kid?\n")
    
    data._name = sure('Name: ')

    echo("Gentleman: Hello, " .. data._name .. ". I am Ottus, I live here since\t well, I always lived here.\n")
    talk = sure("A : what is the city's name?\nB : what are you, Ottus?\n< ", {"a", "b"})
    
    if talk:lower() == 'a' then

        echo("You: What is the city's name?\n")
        data._self = tostring(tonumber(data._self) + 1)

        echo("Ottus: You are in Rainfall, the city which never stops raining.\n")
        echo("\nOttus seems to be happy to share the feeling of getting wet with someone else.\n")
        echo("But in a half-faded inner smile.\n")
    end

    if talk:lower() == 'b' then

        echo("You: what are you, Ottus?\n")
        data._self = tostring(tonumber(data._self) - 1)

        echo("Ottus: Well, I'm a ghost, I guess.\n")
        echo("\nThe ghost looks a little offended, but he seems to be used to it.\n")
    end

    last = talk

    if last == 'a' then
        
        talk = sure("A : Do you have a house? I think is better we talk there.\nB : Let's get out of the rain, I don't wanna get sick.\n< ", {"a", "b"})
        
        if talk:lower() == 'a' then

            echo("You: Do you have a house?% I think is better we talk there.\n")
        end

        if talk:lower() == 'b' then

            echo("You: Let's get out of the rain, I don't wanna get sick.\n")
        end
    end

    if last == 'b' then
        
        talk = sure("A : I'm sorry, I didn't want to offend you.\nB : A ghost, so... are you dead?\n< ", {"a", "b"})
    
        if talk:lower() == 'a' then

            echo("You: I'm sorry, I didn't want to offend you.\n")
            data._self = tostring(tonumber(data._self) + 2)

            echo("Ottus: No, no, it's ok. You are not the first person to ask it.\n")
            echo("\nHe says apologetically, worried about your concern.\n")
        end

        if talk:lower() == 'b' then

            echo("You: A ghost,% so\t are you dead?\n")
            data._self = tostring(tonumber(data._self) - 2)

            echo("Ottus: I belive so.\n")
            echo("\nConfirm him, with a look of who confirms the obvious.\n")
        end
    end

    exit()
end

function sure(question, alt)

    io.write('\n' .. question)
    answ = io.read()

    io.write("\nAre you sure [" .. answ .. "]? (Y\\n)\n< ")
    conf = io.read()

    if alt then

        while  conf:lower() ~= 'y' or not sys.finds(alt, answ:lower()) do
            
            if not sys.finds(alt, answ:lower()) then io.write("\nThis is not an option!\n") end

            io.write('\n' .. question)
            answ = io.read()
    
            io.write("\nAre you sure [" .. answ .. "]? (Y\\n)\n< ")
            conf = io.read()
        end
    
    else
        while conf:lower() ~= 'y' do
            
            io.write('\n' .. question)
            answ = io.read()

            io.write("\nAre you sure [" .. answ .. "]? (Y\\n)\n< ")
            conf = io.read()
        end
    end

    io.write('\n')
    return answ
end

function echo(st)

    local strings = {}
    
    for i = 1, #st do
        
        strings[i] = st:sub(i, i)
    end

    for str in pairs(strings) do
        
        wait(0.08)

        if strings[str] == "%" then wait(0.5)

        elseif strings[str] == "," or strings[str] == "." then
            
            io.write(strings[str])
            wait(0.5)
            io.flush()

        elseif strings[str] == "\n" then

            io.write('\n')
            wait(0.5)

        elseif strings[str] == "\t" then dots()

        else
            io.write(strings[str])
            io.flush()
        end
    end
end


function wait(tm)

    local clock = os.clock
    local ftime = clock()
    while clock() - ftime <= tm do end
end

function dots()

    for i = 1, 3 do
        wait(0.4)
        io.write('.')
        io.flush()
    end
    wait(0.5)
end

function save()

    -- Create file or read it --
    local file = io.open("data.dll", "w")
    local line = ''

    -- Write every value in pairs --
    for itm in pairs(data) do
        
        line = line .. tostring(itm) .. ':' .. tostring(data[itm]) .. '\n'
    end

    -- Remove the last enter (\n) --
    line = line:sub(1, -1)

    -- Write it on file --
    file:write(line)
    file:close()

    io.write('\nYou fill safe.\n')
end

function load()
    
    local splt = nil

    -- This file does not exists --
    if io.open("data.dll") == nil then
        
        print('The main file of the system is missing ("data.dll").')
        exit()
    end

    -- Open and read all lines --
    local file = io.open('data.dll')
    local line = file:read('*all')

    file:close()

    -- Break text on enters --
    line = sys.split(line, '\n')

    -- Read every line --
    for itm in pairs(line) do
        
        -- Get value name and value --
        splt = sys.split(line[itm], ':')

        -- Convert to number if is --
        if tonumber(splt[2]) ~= nil then splt[2] = tonumber(splt[2]) end

        -- Store loaded data --
        data[splt[1]] = splt[2]
    end
end

function exit()

    wait(0.5)
    io.write("\nPress return to exit")
    io.read()
    os.exit()
end

main()