$ErrorActionPreference = "Stop"

Write-Host ""
Write-Host "======================================"
Write-Host " RailDrone GitHub Project Setup"
Write-Host "======================================"
Write-Host ""

# ------------------------------------------------------------
# Verify GitHub CLI
# ------------------------------------------------------------

if (-not (Get-Command gh -ErrorAction SilentlyContinue)) {
    throw "GitHub CLI (gh) is not installed or is not available in PATH."
}

gh auth status

# Automatically detect repository
$repo = gh repo view --json nameWithOwner -q ".nameWithOwner"

if (-not $repo) {
    throw "Could not detect the GitHub repository."
}

Write-Host ""
Write-Host "Repository: $repo"
Write-Host ""

# ------------------------------------------------------------
# LABELS
# ------------------------------------------------------------

$labels = @(
    @{
        Name = "area:firmware"
        Color = "1D76DB"
        Description = "ESP32 firmware and embedded software"
    },
    @{
        Name = "area:control"
        Color = "5319E7"
        Description = "Control algorithms, PID and motor mixing"
    },
    @{
        Name = "area:sensors"
        Color = "0E8A16"
        Description = "IMU, ToF and other sensors"
    },
    @{
        Name = "area:actuators"
        Color = "D93F0B"
        Description = "Motors, ESCs and actuators"
    },
    @{
        Name = "area:interface"
        Color = "006B75"
        Description = "Dashboard and user interface"
    },
    @{
        Name = "area:electronics"
        Color = "FBCA04"
        Description = "Electronics, wiring and power system"
    },
    @{
        Name = "area:mechanical"
        Color = "BFDADC"
        Description = "Mechanical design and CAD"
    },
    @{
        Name = "area:simulation"
        Color = "C5DEF5"
        Description = "Simulation and mathematical models"
    },
    @{
        Name = "area:docs"
        Color = "0075CA"
        Description = "Technical documentation"
    },

    @{
        Name = "priority:high"
        Color = "B60205"
        Description = "High priority"
    },
    @{
        Name = "priority:medium"
        Color = "FBCA04"
        Description = "Medium priority"
    },
    @{
        Name = "priority:low"
        Color = "0E8A16"
        Description = "Low priority"
    }
)

Write-Host "Configuring labels..."

foreach ($label in $labels) {

    gh label create $label.Name `
        --repo $repo `
        --color $label.Color `
        --description $label.Description `
        --force | Out-Null

    Write-Host "  OK  $($label.Name)"
}

# ------------------------------------------------------------
# MILESTONES
# ------------------------------------------------------------

$milestones = @(
    @{
        Title = "M1 - Architecture"
        Description = "System architecture, safety, hardware interfaces and communication contracts."
    },
    @{
        Title = "M2 - Sensors & Actuators"
        Description = "Integration and characterization of sensors, motors, ESCs and physical inputs."
    },
    @{
        Title = "M3 - Manual Control"
        Description = "Manual control of the RailDrone using the PlayStation gamepad."
    },
    @{
        Title = "M4 - Closed Loop Control"
        Description = "Mathematical model, PID controllers and motor mixing."
    },
    @{
        Title = "M5 - Final Integrated System"
        Description = "Dashboard, integration, validation and final demonstration."
    }
)

Write-Host ""
Write-Host "Configuring milestones..."

$existingMilestones = gh api `
    "repos/$repo/milestones?state=all&per_page=100" |
    ConvertFrom-Json

foreach ($milestone in $milestones) {

    $exists = $existingMilestones |
        Where-Object { $_.title -eq $milestone.Title }

    if (-not $exists) {

        gh api `
            --method POST `
            "repos/$repo/milestones" `
            -f "title=$($milestone.Title)" `
            -f "description=$($milestone.Description)" |
            Out-Null

        Write-Host "  CREATED  $($milestone.Title)"
    }
    else {
        Write-Host "  EXISTS   $($milestone.Title)"
    }
}

# ------------------------------------------------------------
# ISSUES
# ------------------------------------------------------------

$issues = @(

    @{
        Title = "Define hardware baseline and system I/O map"
        Milestone = "M1 - Architecture"
        Labels = @(
            "area:electronics",
            "area:firmware",
            "priority:high"
        )
        Body = @"
## Objective

Define the initial hardware baseline of the RailDrone Control System and document how every subsystem connects to the ESP32.

## Components

- ESP32 DevKit
- BNO085 IMU
- ToF sensor
- Limit switches
- BLDC motors
- ESCs
- Propellers
- PlayStation controller
- Battery
- Voltage regulation
- Safety system

## Deliverables

- [ ] Confirm hardware baseline
- [ ] Define ESP32 pin assignments
- [ ] Define communication buses
- [ ] Define ESC outputs
- [ ] Define limit switch inputs
- [ ] Define battery monitoring
- [ ] Define gamepad communication
- [ ] Create initial I/O map
- [ ] Document unresolved decisions

## Acceptance criteria

An initial hardware configuration and I/O map are documented and can be used for firmware development.
"@
    },

    @{
        Title = "Define safety state machine and fault handling"
        Milestone = "M1 - Architecture"
        Labels = @(
            "area:firmware",
            "area:control",
            "priority:high"
        )
        Body = @"
## Objective

Define the operating states and safety behavior of the RailDrone.

## Initial states

- BOOT
- SELF_TEST
- DISARMED
- ARMED
- RUNNING
- FAULT
- EMERGENCY_STOP

## Deliverables

- [ ] Define valid state transitions
- [ ] Define arming requirements
- [ ] Define sensor failure behavior
- [ ] Define gamepad disconnect behavior
- [ ] Define limit switch behavior
- [ ] Define emergency stop behavior
- [ ] Define recovery from faults

## Acceptance criteria

A documented state machine exists with deterministic behavior for normal and fault conditions.
"@
    },

    @{
        Title = "Define telemetry protocol"
        Milestone = "M1 - Architecture"
        Labels = @(
            "area:firmware",
            "area:interface",
            "area:docs",
            "priority:medium"
        )
        Body = @"
## Objective

Define the data contract between the ESP32 and the monitoring interface.

## Initial telemetry

- Timestamp
- System state
- Control mode
- Altitude
- Altitude reference
- Angle
- Angle reference
- Motor outputs
- Battery voltage
- IMU status
- ToF status
- Limit switches
- Controller status

## Deliverables

- [ ] Define telemetry fields
- [ ] Define units
- [ ] Define serialization format
- [ ] Define update rate
- [ ] Document protocol

## Acceptance criteria

Firmware and dashboard can implement telemetry independently using the documented contract.
"@
    },

    @{
        Title = "Integrate BNO085 IMU"
        Milestone = "M2 - Sensors & Actuators"
        Labels = @(
            "area:sensors",
            "area:firmware",
            "priority:high"
        )
        Body = @"
## Objective

Integrate the BNO085 IMU with the ESP32.

## Deliverables

- [ ] Establish communication
- [ ] Read orientation
- [ ] Define coordinate convention
- [ ] Extract required angular variable
- [ ] Detect communication failure
- [ ] Validate measurement rate
- [ ] Document wiring

## Acceptance criteria

The ESP32 provides stable and repeatable orientation measurements suitable for control.
"@
    },

    @{
        Title = "Integrate ToF height sensor"
        Milestone = "M2 - Sensors & Actuators"
        Labels = @(
            "area:sensors",
            "area:firmware",
            "priority:high"
        )
        Body = @"
## Objective

Integrate the Time-of-Flight sensor used to estimate RailDrone height.

## Deliverables

- [ ] Establish communication
- [ ] Read distance
- [ ] Convert measurement to system height
- [ ] Handle invalid readings
- [ ] Determine useful measurement range
- [ ] Characterize update rate
- [ ] Document wiring

## Acceptance criteria

Reliable height measurements are available to the control system.
"@
    },

    @{
        Title = "Integrate limit switches and safety inputs"
        Milestone = "M2 - Sensors & Actuators"
        Labels = @(
            "area:sensors",
            "area:electronics",
            "area:firmware",
            "priority:high"
        )
        Body = @"
## Objective

Implement physical travel limits and safety inputs.

## Deliverables

- [ ] Integrate upper limit switch
- [ ] Integrate lower limit switch
- [ ] Define pull-up or pull-down configuration
- [ ] Implement debouncing
- [ ] Integrate inputs with safety manager
- [ ] Test fault behavior

## Acceptance criteria

Physical limits override normal control commands safely and deterministically.
"@
    },

    @{
        Title = "Implement ESC and motor output abstraction"
        Milestone = "M2 - Sensors & Actuators"
        Labels = @(
            "area:actuators",
            "area:firmware",
            "area:electronics",
            "priority:high"
        )
        Body = @"
## Objective

Implement a reusable firmware interface for controlling the ESCs and BLDC motors.

## Deliverables

- [ ] Confirm ESC control protocol
- [ ] Implement initialization
- [ ] Implement motor command interface
- [ ] Define minimum and maximum outputs
- [ ] Implement output saturation
- [ ] Implement safe motor stop
- [ ] Perform bench test

## Acceptance criteria

Motor commands can be generated safely and consistently from the ESP32.
"@
    },

    @{
        Title = "Implement PlayStation gamepad input"
        Milestone = "M3 - Manual Control"
        Labels = @(
            "area:firmware",
            "priority:high"
        )
        Body = @"
## Objective

Use a PlayStation controller as the main operator input.

## Initial controls

- Height command
- Angular command
- Arm
- Disarm
- Emergency stop
- Mode selection

## Deliverables

- [ ] Connect controller to ESP32
- [ ] Read joystick inputs
- [ ] Implement dead zones
- [ ] Map buttons
- [ ] Detect disconnect
- [ ] Integrate with safety state machine

## Acceptance criteria

The operator can safely command the prototype using the gamepad.
"@
    },

    @{
        Title = "Develop mathematical model of RailDrone dynamics"
        Milestone = "M4 - Closed Loop Control"
        Labels = @(
            "area:control",
            "area:simulation",
            "priority:high"
        )
        Body = @"
## Objective

Develop an initial mathematical model for vertical and angular dynamics.

## Variables

- Height z
- Vertical velocity
- Angle theta
- Angular velocity
- Motor thrusts
- System mass
- Moment of inertia

## Deliverables

- [ ] Define coordinate system
- [ ] Draw free-body diagram
- [ ] Derive vertical dynamics
- [ ] Derive angular dynamics
- [ ] Identify required parameters
- [ ] Document assumptions
- [ ] Build initial simulation

## Acceptance criteria

The model is sufficient to guide controller design and experimental identification.
"@
    },

    @{
        Title = "Implement reusable PID controller module"
        Milestone = "M4 - Closed Loop Control"
        Labels = @(
            "area:control",
            "area:firmware",
            "priority:high"
        )
        Body = @"
## Objective

Implement a generic PID controller that can be reused for altitude and angle control.

## Deliverables

- [ ] Implement proportional term
- [ ] Implement integral term
- [ ] Implement derivative term
- [ ] Add output limits
- [ ] Add integral anti-windup
- [ ] Support fixed sample time
- [ ] Add basic tests

## Acceptance criteria

The PID implementation is reusable and independent from specific sensors or motors.
"@
    },

    @{
        Title = "Implement altitude, angle controllers and motor mixer"
        Milestone = "M4 - Closed Loop Control"
        Labels = @(
            "area:control",
            "area:firmware",
            "area:actuators",
            "priority:high"
        )
        Body = @"
## Objective

Combine altitude and angular control outputs into individual motor commands.

## Control variables

Altitude:

e_z = z_ref - z

Angle:

e_theta = theta_ref - theta

## Initial mixer concept

Motor 1 = base + altitude + angle

Motor 2 = base + altitude - angle

## Deliverables

- [ ] Implement altitude controller
- [ ] Implement angle controller
- [ ] Implement motor mixer
- [ ] Implement saturation
- [ ] Define control limits
- [ ] Validate sign conventions
- [ ] Test combined control

## Acceptance criteria

Altitude and angular correction commands can be translated into valid motor outputs.
"@
    },

    @{
        Title = "Build telemetry dashboard and integrated system validation"
        Milestone = "M5 - Final Integrated System"
        Labels = @(
            "area:interface",
            "area:firmware",
            "priority:medium"
        )
        Body = @"
## Objective

Build the monitoring dashboard and perform the initial complete-system validation.

## Dashboard

Display:

- Altitude
- Altitude reference
- Angle
- Angle reference
- Motor outputs
- Battery voltage
- Sensor status
- Limit switches
- Gamepad status
- System state
- Control mode

## Validation

- [ ] Verify telemetry communication
- [ ] Plot altitude response
- [ ] Plot angular response
- [ ] Test manual mode
- [ ] Test closed-loop mode
- [ ] Test safety states
- [ ] Document final results

## Acceptance criteria

The complete system can be monitored and demonstrated through the user interface.
"@
    }
)

Write-Host ""
Write-Host "Configuring issues..."

$existingIssues = gh issue list `
    --repo $repo `
    --state all `
    --limit 200 `
    --json number,title |
    ConvertFrom-Json

foreach ($issue in $issues) {

    $exists = $existingIssues |
        Where-Object { $_.title -eq $issue.Title }

    if ($exists) {
        Write-Host "  EXISTS   $($issue.Title)"
        continue
    }

    $arguments = @(
        "issue",
        "create",
        "--repo", $repo,
        "--title", $issue.Title,
        "--body", $issue.Body,
        "--milestone", $issue.Milestone
    )

    foreach ($label in $issue.Labels) {
        $arguments += @("--label", $label)
    }

    & gh @arguments | Out-Null

    Write-Host "  CREATED  $($issue.Title)"
}

Write-Host ""
Write-Host "======================================"
Write-Host " GitHub setup completed successfully"
Write-Host "======================================"
Write-Host ""
Write-Host "Repository:"
Write-Host "  $repo"
Write-Host ""
Write-Host "Milestones:"
foreach ($milestone in $milestones) {
    Write-Host "  - $($milestone.Title)"
}
Write-Host ""
Write-Host "Initial backlog:"
Write-Host "  $($issues.Count) issues"
Write-Host ""