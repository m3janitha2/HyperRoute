#include <iostream>
#include <array>
#include <concepts>
#include <type_traits>

namespace hyper::framework
{
    template <typename StateType>
    concept StateTypeConcept = requires {
        { StateType::State::Invalid };
    };

    template <StateTypeConcept StateType, size_t NumberOfStates = static_cast<size_t>(StateType::State::Invalid) + 1>
    class StateMachine
    {
    public:
        using State = typename StateType::State;
        
        StateMachine(const StateMachine &) = delete;
        StateMachine &operator=(const StateMachine &) = delete;

        explicit constexpr StateMachine(State initial_state)
            : current_state_(initial_state) {}

        constexpr State current_state() const noexcept
        {
            return current_state_;
        }

        constexpr bool update_state(State to)
        {
            if (!is_valid_transition(current_state_, to)) [[unlikely]]
            {
                return false;
            }
            current_state_ = to;
            return true;
        }

        template <State From, State To>
        static consteval void configure_valid_transition()
        {
            transition_table[static_cast<size_t>(From)][static_cast<size_t>(To)] = true;
        }

    private:
        constexpr static bool is_valid_transition(State from, State to)
        {
            return transition_table[static_cast<size_t>(from)][static_cast<size_t>(to)];
        }

        State current_state_;
        using TransitionTableType = std::array<std::array<bool, NumberOfStates>, NumberOfStates>;
        static inline constinit TransitionTableType transition_table = {};
    };

    struct ServerSessionState
    {
        enum class State : int
        {
            Disconnected,
            Connected,
            LoginRequestReceived,
            LoginAckSent,
            Operational,
            LogoutRequestSent,
            LogoutAckReceived,
            LogoutRequestReceived,
            LogoutAckSent,
            Recovery,
            Invalid
        };
    };

    class ServerSessionStateMachine : public StateMachine<ServerSessionState>
    {
    public:
        explicit constexpr ServerSessionStateMachine(ServerSessionState::State initial_state = ServerSessionState::State::Disconnected)
            : StateMachine(initial_state)
        {
            using enum ServerSessionState::State;

            // Configure login transitions
            configure_valid_transition<Disconnected, Connected>();
            configure_valid_transition<Connected, LoginRequestReceived>();
            configure_valid_transition<LoginRequestReceived, LoginAckSent>();
            configure_valid_transition<LoginAckSent, Operational>();

            // Configure logout transitions (initiated by server)
            configure_valid_transition<Operational, LogoutRequestSent>();
            configure_valid_transition<LogoutRequestSent, LogoutAckReceived>();
            configure_valid_transition<LogoutAckReceived, Disconnected>();

            // Configure logout transitions (initiated by counterparty)
            configure_valid_transition<Operational, LogoutRequestReceived>();
            configure_valid_transition<LogoutRequestReceived, LogoutAckSent>();
            configure_valid_transition<LogoutAckSent, Disconnected>();

            // Recovery transition
            configure_valid_transition<Disconnected, Recovery>();
            configure_valid_transition<Recovery, Operational>();
        }
    };
}
