class FrameTime
	attr_accessor :minutes
	attr_accessor :seconds
	def initialize(minutes, seconds)
		@minutes = minutes
		@seconds = seconds
	end
	def to_s
		return "%d %02d" % [@minutes, @seconds]
	end
end

class FrameInput
	attr_accessor :value
	def initialize(value)
		@value = value
	end

	def to_s
		return "%04x" % @value
	end
end

class FrameState
	attr_accessor :frame
	attr_accessor :input
	attr_accessor :trigger
	attr_accessor :repeat
	attr_accessor :time
	attr_accessor :timer
	attr_accessor :x
	attr_accessor :y
	attr_accessor :score
	attr_accessor :level
	attr_accessor :next
	attr_accessor :combo
	attr_accessor :chain
	attr_accessor :timeout
	attr_accessor :counter
	attr_accessor :rise
	attr_accessor :speed
	attr_accessor :states
	attr_accessor :panels

	def write(file)
		file.puts('frame start')
		file.puts("input #{@input} #{@trigger} #{@repeat}")
		file.puts("time #{@time}")
		file.puts("timer #{@timer}")
		file.puts("selector #{@y} #{@x}")
		file.puts("score #{@score}")
		file.puts("level #{@level}")
		file.puts("next #{@next}")
		file.puts("combo #{@combo}")
		file.puts("chain #{@chain}")
		file.puts("timeout #{@timeout}")
		file.puts("rise #{@rise}")
		file.puts("counter #{'%04x' % @counter}")
		file.puts("speed #{'%04x' % @speed}")
		file.puts("states #{@states.join(' ')}")
		13.times do |i|
			6.times do |j|
				id = i * 6 + j
				file.print("%08x " % @panels[id])
			end
			file.print("\n")
		end
	end
end

def read_input(file)
	input = file.readline
	raise 'input' if not input.include?('input')
	tokens = input.split(' ')
	tokens.shift
	return tokens.collect {|val| FrameInput.new(val.to_i(16))}
end

def read_time(file, attribute)
	time = file.readline
	raise attribute if not time.include?(attribute)
    tokens = time.split(' ')
	return FrameTime.new(tokens[1].to_i, tokens[2].to_i)
end

def read_point(file, attribute)
	point = file.readline
	raise attribute if not point.include?(attribute)
	tokens = point.split(' ')
	tokens.shift
	return tokens.collect {|i| i.to_i}
end

def read_int(file, attribute, base=10)
	val = file.readline
	raise attribute if not val.include?(attribute)
	tokens = val.split(' ')
	return tokens[1].to_i(base)
end

def read_array_int(file, attribute, base=10)
	val = file.readline
	raise attribute if not val.include?(attribute)
	tokens = val.split(' ')
	tokens.shift
	return tokens.collect {|e| e.to_i(base)}
end

def read_panels(file)
	data = []
	13.times do |i|
		values = file.readline.split
		values.collect! {|i| i.to_i(16)}
		data += values
	end
	return data
end

def read_frames(file)
	frames = []
	frame = 0
	until file.eof?
		state = FrameState.new
		line = file.readline.strip
		raise 'Frame Start not found' if line != 'frame start'

		state.frame = frame
		state.input, state.trigger, state.repeat = *read_input(file)
		state.time = read_time(file, 'time')
        state.timer = read_time(file, 'timer')
		state.y, state.x = *read_point(file, 'selector')
        state.score = read_int(file, 'score')
		state.level = read_int(file, 'level')
		state.next = read_int(file, 'next')
		state.combo = read_int(file, 'combo')
		state.chain = read_int(file, 'chain')
		state.timeout = read_int(file, 'timeout')
		state.rise = read_int(file, 'rise')
		state.counter = read_int(file, 'counter', 16)
		state.speed = read_int(file, 'speed', 16)
		state.states = read_array_int(file, 'states')
		state.panels = read_panels(file)
		frames << state
		file.readline
		file.readline
        frame += 1
	end
	return frames
end

def write_frames(file, frames)
	frames.each do |frame|
		frame.write(file)
	    file.print("\n\n")
	end

end

def clean_frames(frames)
	remove_frames = []
	frames.each_with_index do |frame, index|
		next if index == 0
		before = frames[index - 1]
		if before.panels[72] == 0xff00ff and frame.panels[72] == 0xff00ff
			remove_frames << index - 1
		end
	end

	clean = []
	# Search for last frame with 0fff as counter.  It will be declared the initial frame.
	initial = -1
	frames.each_with_index do |frame, index|
		next if index == 0
		last = frames[index - 1]
		if last.counter == 0xfff and frame.counter != 0xfff
			initial = index - 1
			break
		end
	end
	raise 'Initial frame not found' if initial == -1

	# Final frame is when clock is 2 0 and all panels 3rd byte has 10.
	final = -1
	initial.upto(frames.size - 1) do |index|
		frame = frames[index]
		if frame.time.minutes == 2 and frame.time.seconds == 0
			good = true
			72.times do |index|
				p = frame.panels[index]
				# If it is empty then ok, or if it is not empty and state flags are x10
				unless ((p & 0xFF) == 0) or ((p & 0xFF00) == 0x1000)
					good = false
					break
				end
			end
			if good
				final = index
				break
			end
		end
	end
	final = frames.size if final == -1
	
	clean = frames.slice(initial...final)
	clean.reject! {|state| remove_frames.include?(state.frame)}
	return clean
end

def main
	file = File.open(ARGV[0], 'rb')
	frames = read_frames(file)

	cleaned = clean_frames(frames)

	file = File.new(ARGV[1], 'wb')
	write_frames(file, cleaned)
end

main
