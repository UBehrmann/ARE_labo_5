------------------------------------------------------------------------------------------
-- HEIG-VD ///////////////////////////////////////////////////////////////////////////////
-- Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
-- School of Business and Engineering in Canton de Vaud
------------------------------------------------------------------------------------------
-- REDS Institute ////////////////////////////////////////////////////////////////////////
-- Reconfigurable Embedded Digital Systems
------------------------------------------------------------------------------------------
--
-- File                 : avl_user_interface.vhd
-- Author               : Anthony Convers
-- Date                 : 04.08.2022
--
-- Context              : Avalon user interface
--
------------------------------------------------------------------------------------------
-- Description : 
--   
------------------------------------------------------------------------------------------
-- Dependencies : 
--   
------------------------------------------------------------------------------------------
-- Modifications :
-- Ver    Date        Engineer    Comments
-- 0.0    See header              Initial version
-- 1.0    29.11.2024  UB          first version

------------------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY avl_user_interface IS
  PORT (
    -- Avalon bus
    avl_clk_i : IN STD_LOGIC;
    avl_reset_i : IN STD_LOGIC;
    avl_address_i : IN STD_LOGIC_VECTOR(13 DOWNTO 0);
    avl_byteenable_i : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
    avl_write_i : IN STD_LOGIC;
    avl_writedata_i : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    avl_read_i : IN STD_LOGIC;
    avl_readdatavalid_o : OUT STD_LOGIC;
    avl_readdata_o : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    avl_waitrequest_o : OUT STD_LOGIC;
    -- User interface
    button_i : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
    switch_i : IN STD_LOGIC_VECTOR(9 DOWNTO 0);
    led_o : OUT STD_LOGIC_VECTOR(9 DOWNTO 0);
    -- Gen strings
    char_1_i : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    char_2_i : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    char_3_i : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    char_4_i : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    char_5_i : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    char_6_i : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    char_7_i : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    char_8_i : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    char_9_i : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    char_10_i : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    char_11_i : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    char_12_i : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    char_13_i : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    char_14_i : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    char_15_i : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    char_16_i : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    checksum_i : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    cmd_init_o : OUT STD_LOGIC;
    cmd_new_char_o : OUT STD_LOGIC;
    auto_o : OUT STD_LOGIC;
    delay_o : OUT STD_LOGIC_VECTOR(1 DOWNTO 0)
  );
END avl_user_interface;

ARCHITECTURE rtl OF avl_user_interface IS

  --| Components declaration |--------------------------------------------------------------
  --| Constants declarations |--------------------------------------------------------------
  CONSTANT INTERFACE_ID_C : STD_LOGIC_VECTOR(31 DOWNTO 0) := x"12345678";
  CONSTANT OTHERS_VAL_C : STD_LOGIC_VECTOR(31 DOWNTO 0) := x"00000000";
  CONSTANT ID_ADDRESS : INTEGER := 0;
  CONSTANT BOUTTON_ADDRESS : INTEGER := 1;
  CONSTANT SWITCH_ADDRESS : INTEGER := 2;
  CONSTANT LED_ADDRESS : INTEGER := 3;
  CONSTANT STATUS_ADDRESS : INTEGER := 4;
  CONSTANT NEW_CHAR_INIT_CHAR_ADDRESS : INTEGER := 4;
  CONSTANT MODE_GEN_AND_DELAY_GEN_ADDRESS : INTEGER := 5;
  CONSTANT SAVE_CHAR_ADDRESS : INTEGER := 6;
  CONSTANT RELIABLE_ADDRESS : INTEGER := 7;
  CONSTANT CHAR_1_TO_4_ADDRESS : INTEGER := 8;
  CONSTANT CHAR_5_TO_8_ADDRESS : INTEGER := 9;
  CONSTANT CHAR_9_TO_12_ADDRESS : INTEGER := 10;
  CONSTANT CHAR_13_TO_16_ADDRESS : INTEGER := 11;
  CONSTANT CHECKSUM_ADDRESS : INTEGER := 12;

  --| Signals declarations   |--------------------------------------------------------------   
  SIGNAL led_reg_s : STD_LOGIC_VECTOR(9 DOWNTO 0);

  SIGNAL status_s : STD_LOGIC_VECTOR(1 DOWNTO 0);

  SIGNAL mode_gen_s : STD_LOGIC;
  SIGNAL delay_gen_s : STD_LOGIC_VECTOR(1 DOWNTO 0);

  SIGNAL checksum_s : STD_LOGIC_VECTOR(7 DOWNTO 0);

  SIGNAL new_char_s : STD_LOGIC;
  SIGNAL init_char_s : STD_LOGIC;

  SIGNAL readdatavalid_next_s : STD_LOGIC;
  SIGNAL readdatavalid_reg_s : STD_LOGIC;
  SIGNAL readdata_next_s : STD_LOGIC_VECTOR(31 DOWNTO 0);
  SIGNAL readdata_reg_s : STD_LOGIC_VECTOR(31 DOWNTO 0);

  SIGNAL boutton_s : STD_LOGIC_VECTOR(3 DOWNTO 0);
  SIGNAL switches_s : STD_LOGIC_VECTOR(9 DOWNTO 0);

  -- Partie 2
  SIGNAL reliable_s : STD_LOGIC;
  SIGNAL save_s : STD_LOGIC;
  SIGNAL save_char_s : STD_LOGIC;
  SIGNAL char_rdy_s : STD_LOGIC;

  SIGNAL chars_0_s : STD_LOGIC_VECTOR(31 DOWNTO 0);
  SIGNAL chars_1_s : STD_LOGIC_VECTOR(31 DOWNTO 0);
  SIGNAL chars_2_s : STD_LOGIC_VECTOR(31 DOWNTO 0);
  SIGNAL chars_3_s : STD_LOGIC_VECTOR(31 DOWNTO 0);

  SIGNAL reg_chars_0_s : STD_LOGIC_VECTOR(31 DOWNTO 0);
  SIGNAL reg_chars_1_s : STD_LOGIC_VECTOR(31 DOWNTO 0);
  SIGNAL reg_chars_2_s : STD_LOGIC_VECTOR(31 DOWNTO 0);
  SIGNAL reg_chars_3_s : STD_LOGIC_VECTOR(31 DOWNTO 0);
  SIGNAL reg_checksum_s : STD_LOGIC_VECTOR(7 DOWNTO 0);

  SIGNAL to_send_chars_0_s : STD_LOGIC_VECTOR(31 DOWNTO 0);
  SIGNAL to_send_chars_1_s : STD_LOGIC_VECTOR(31 DOWNTO 0);
  SIGNAL to_send_chars_2_s : STD_LOGIC_VECTOR(31 DOWNTO 0);
  SIGNAL to_send_chars_3_s : STD_LOGIC_VECTOR(31 DOWNTO 0);
  SIGNAL to_send_checksum_s : STD_LOGIC_VECTOR(7 DOWNTO 0);

  --| Types |----------------------------------------------------------------
  
  TYPE state_t IS (
    --General state
    WAIT_FOR_SAVE,
    SAVE,
    SAVE_DONE
  );
  SIGNAL e_pres, e_fut_s : state_t;

BEGIN

  -- Input signals

  -- Synchronisation des signaux d'entree
  sync_input : PROCESS (
    avl_reset_i,
    avl_clk_i
    )
  BEGIN
    IF avl_reset_i = '1' THEN

      boutton_s <= (OTHERS => '0');
      switches_s <= (OTHERS => '0');

    ELSIF rising_edge(avl_clk_i) THEN

      boutton_s <= button_i;
      switches_s <= switch_i;

    END IF;
  END PROCESS;

  -- Output signals

  led_o <= led_reg_s;

  avl_readdatavalid_o <= readdatavalid_reg_s;
  avl_readdata_o <= readdata_reg_s;

  -- Read access part
  -- Read register process

  read_decoder_p : PROCESS (
    avl_read_i,
    avl_address_i,
    boutton_s,
    switches_s,
    led_reg_s,
    status_s,
    mode_gen_s,
    delay_gen_s,
    reliable_s,
    to_send_chars_0_s,
    to_send_chars_1_s,
    to_send_chars_2_s,
    to_send_chars_3_s,
    to_send_checksum_s
    )
  BEGIN
    readdatavalid_next_s <= '0'; --valeur par defaut
    readdata_next_s <= (OTHERS => '0'); --valeur par defaut

    IF avl_read_i = '1' THEN
      readdatavalid_next_s <= '1';

      CASE (to_integer(unsigned(avl_address_i))) IS

        WHEN ID_ADDRESS =>
          readdata_next_s <= INTERFACE_ID_C;

        WHEN BOUTTON_ADDRESS =>
          readdata_next_s(3 DOWNTO 0) <= boutton_s;

        WHEN SWITCH_ADDRESS =>
          readdata_next_s(9 DOWNTO 0) <= switches_s;

        WHEN LED_ADDRESS =>
          readdata_next_s(9 DOWNTO 0) <= led_reg_s;

        WHEN STATUS_ADDRESS =>
          readdata_next_s(1 DOWNTO 0) <= status_s;

        WHEN MODE_GEN_AND_DELAY_GEN_ADDRESS =>
          readdata_next_s(4) <= mode_gen_s;
          readdata_next_s(1 DOWNTO 0) <= delay_gen_s;

        WHEN RELIABLE_ADDRESS =>
          readdata_next_s(0) <= reliable_s;

        WHEN CHAR_1_TO_4_ADDRESS =>
          readdata_next_s <= to_send_chars_0_s;

        WHEN CHAR_5_TO_8_ADDRESS =>
          readdata_next_s <= to_send_chars_1_s;

        WHEN CHAR_9_TO_12_ADDRESS =>
          readdata_next_s <= to_send_chars_2_s;

        WHEN CHAR_13_TO_16_ADDRESS =>
          readdata_next_s <= to_send_chars_3_s;

        WHEN CHECKSUM_ADDRESS =>
          readdata_next_s(7 DOWNTO 0) <= to_send_checksum_s;

        WHEN OTHERS =>
          readdata_next_s <= OTHERS_VAL_C;

      END CASE;
    END IF;
  END PROCESS;

  -- Read register process

  read_register_p : PROCESS (
    avl_reset_i,
    avl_clk_i
    )
  BEGIN
    IF avl_reset_i = '1' THEN

      readdatavalid_reg_s <= '0';
      readdata_reg_s <= (OTHERS => '0');

    ELSIF rising_edge(avl_clk_i) THEN

      readdatavalid_reg_s <= readdatavalid_next_s;
      readdata_reg_s <= readdata_next_s;

    END IF;
  END PROCESS;

  -- Write access part

  write_register_p : PROCESS (
    avl_reset_i,
    avl_clk_i
    )
  BEGIN

    IF avl_reset_i = '1' THEN

      led_reg_s <= (OTHERS => '0');
      new_char_s <= '0';
      init_char_s <= '0';
      mode_gen_s <= '0';
      delay_gen_s <= (OTHERS => '0');

    ELSIF rising_edge(avl_clk_i) THEN

      new_char_s <= '0';
      init_char_s <= '0';
      save_char_s <= '0';

      IF avl_write_i = '1' THEN

        CASE (to_integer(unsigned(avl_address_i))) IS

          WHEN LED_ADDRESS =>
            led_reg_s <= avl_writedata_i(9 DOWNTO 0);

          WHEN NEW_CHAR_INIT_CHAR_ADDRESS =>
            new_char_s <= avl_writedata_i(4);
            init_char_s <= avl_writedata_i(0);

          WHEN MODE_GEN_AND_DELAY_GEN_ADDRESS =>
            mode_gen_s <= avl_writedata_i(4);
            delay_gen_s <= avl_writedata_i(1 DOWNTO 0);

          WHEN SAVE_CHAR_ADDRESS =>
            save_char_s <= avl_writedata_i(0);

          WHEN RELIABLE_ADDRESS =>
            reliable_s <= avl_writedata_i(0);

          WHEN OTHERS =>
            NULL;

        END CASE;
      END IF;
    END IF;
  END PROCESS;

  -- Interface management

  cmd_init_o <= init_char_s;
  cmd_new_char_o <= new_char_s;
  auto_o <= mode_gen_s;
  delay_o <= delay_gen_s;

  -- Group chars
  chars_0_s <= char_1_i & char_2_i & char_3_i & char_4_i;
  chars_1_s <= char_5_i & char_6_i & char_7_i & char_8_i;
  chars_2_s <= char_9_i & char_10_i & char_11_i & char_12_i;
  chars_3_s <= char_13_i & char_14_i & char_15_i & char_16_i;

  checksum_s <= checksum_i;

  -- Select chars to send
  to_send_chars_0_s <= chars_0_s WHEN reliable_s = '0' ELSE
    reg_chars_0_s;
  to_send_chars_1_s <= chars_1_s WHEN reliable_s = '0' ELSE
    reg_chars_1_s;
  to_send_chars_2_s <= chars_2_s WHEN reliable_s = '0' ELSE
    reg_chars_2_s;
  to_send_chars_3_s <= chars_3_s WHEN reliable_s = '0' ELSE
    reg_chars_3_s;
  to_send_checksum_s <= checksum_i WHEN reliable_s = '0' ELSE
    reg_checksum_s;
  status_s <= reliable_s & char_rdy_s WHEN reliable_s = '1' ELSE
    "00";

  -- Save chars
  sync_register_p : PROCESS (
    avl_reset_i,
    avl_clk_i
    )
  BEGIN
    IF avl_reset_i = '1' THEN

      reg_chars_0_s <= (OTHERS => '0');
      reg_chars_1_s <= (OTHERS => '0');
      reg_chars_2_s <= (OTHERS => '0');
      reg_chars_3_s <= (OTHERS => '0');

    ELSIF rising_edge(avl_clk_i) THEN

      IF (save_s = '1') THEN
        reg_chars_0_s <= chars_0_s;
        reg_chars_1_s <= chars_1_s;
        reg_chars_2_s <= chars_2_s;
        reg_chars_3_s <= chars_3_s;
        reg_checksum_s <= checksum_s;
      END IF;

    END IF;
  END PROCESS;

  -- State machine
  -- This process update the state of the state machine
  fsm_reg : PROCESS (avl_reset_i, avl_clk_i) IS
  BEGIN
    IF (avl_reset_i = '1') THEN
      e_pres <= WAIT_FOR_SAVE;
    ELSIF (rising_edge(avl_clk_i)) THEN
      e_pres <= e_fut_s;
    END IF;
  END PROCESS fsm_reg;

  dec_fut_sort : PROCESS (
    e_pres,
    save_char_s
    ) IS
  BEGIN
    -- Default values for generated signal
    save_s <= '0';
    char_rdy_s <= '0';
    e_fut_s <= WAIT_FOR_SAVE;

    CASE e_pres IS
      WHEN WAIT_FOR_SAVE =>
        IF save_char_s = '1' THEN
          e_fut_s <= SAVE;
        END IF;

      WHEN SAVE =>
        save_s <= '1';

        e_fut_s <= SAVE_DONE;

      WHEN SAVE_DONE =>
        char_rdy_s <= '1';

        IF save_char_s = '1' THEN
          e_fut_s <= SAVE;
        ELSE
          e_fut_s <= SAVE_DONE;
        END IF;

      WHEN OTHERS =>
        e_fut_s <= WAIT_FOR_SAVE;

    END CASE;
  END PROCESS dec_fut_sort;
END rtl;